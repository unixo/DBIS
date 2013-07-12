/*! \file db_util.c 
 * \author Ferruccio Vitale (unixo@devzero.it) 
 * \date 19/01/2010 
 *
 * \note 
 * Applied Computer Science - Urbino University "Carlo Bo"\n 
 * Databases and information systems\n Professor Alessandro Aldini\n 
 * Academic Year 2009 - 2010 
 */
#include "db_util.h"

/*
 * Database connection parameters
 */
#define DB_HOST     "localhost"
#define DB_USER     "root"
#define DB_PASSWD   ""
#define DB_NAME     "dbis"

#define MIN(a, b)   ((a<b)?a:b)

typedef struct _col_info {
    char           *fmt;
    int             size;
} col_info_t;

/*! \var MYSQL *db_conn
 *  \brief Database connection handle
 *  \details Opaque variable used by mySQL libraries 
 */
MYSQL          *db_conn;

/*! \fn void db_connect()
 *  \brief Connect to database
 *  \details This function is responsible to create a connection to the 
 *  database or display a message error whenever mySQL is not available or
 *  authentication process fails. 
 */
void
db_connect()
{
    db_conn = mysql_init(NULL);
    if (db_conn == NULL) {
        fprintf(stderr, "[ERR] mySQL framework init failed\n");
        exit(1);
    }

    if (mysql_real_connect(db_conn, DB_HOST, DB_USER, DB_PASSWD, DB_NAME,
                           0, NULL, CLIENT_MULTI_STATEMENTS) == NULL) {
        fprintf(stderr, "[ERR] unable to contact mySQL server or "
                "authentication failed\n");
        exit(1);;
    }
}

/*! \fn void db_close()
 *  \brief Close a database connection
 *  \details This function is responsible to close a database connection if 
 *  present. 
 */
void
db_close()
{
    if (db_conn)
        mysql_close(db_conn);
    db_conn = NULL;
}

/*! \fn void db_last_id()
 *  \brief Returns last insert ID
 *  \details After an INSERT command, calling this function, you'll obtain 
 *  the last auto_increment id 
 */
my_ulonglong
db_last_id()
{
    if (!db_conn)
        return (my_ulonglong) - 1;

    return mysql_insert_id(db_conn);
}

/*! \fn int db_query_with_parameter(char *query, char *param, int
 * print_result) 
 *  \brief Execute a query
 *  \details This function is responsible to issue the SQL instruction(s) and 
 *  check if it is successful.
 *  \param query        SQL instruction to issue
 *  \param param        Param to substitue to query (if any)
 *  \param print_result if equals to 1, the result set is printed
 *  \return Status of the operation 
 */
int
db_query_with_parameter(char *query, char *param, int print_result)
{
    char           *str = NULL;
    int             count = 0,
        ret = 0;

    if (!query || !db_conn)
        return 0;

    count = strlen(query);
    if (param) {
        count += strlen(param) + 1;
        str = (char *) calloc(count, sizeof(char));
        snprintf(str, count, query, param);
    } else {
        str = strdup(query);
    }

    ret = mysql_query(db_conn, str);
    free(str);

    if (ret != 0) {
        fprintf(stderr, "ERRORE: %s\n", mysql_error(db_conn));
    } else if (print_result == 1) {
        MYSQL_RES      *res = mysql_use_result(db_conn);
        unsigned int    i,
                        fields_count,
                        width = 0;
        MYSQL_FIELD    *fields;
        MYSQL_ROW       row;
        col_info_t     *cols;

        fields_count = mysql_num_fields(res);
        cols = (col_info_t *) malloc(fields_count * sizeof(col_info_t));
        fields = mysql_fetch_fields(res);

        for (i = 0; i < fields_count; i++) {
            switch (fields[i].type) {
            case MYSQL_TYPE_DATETIME:
                cols[i].fmt = strdup("| % 19.*s ");
                cols[i].size = 19;
                width += 22;
                break;
            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_FLOAT:
                cols[i].fmt = strdup("| % 7.*s ");
                cols[i].size = 7;
                width += 10;
                break;
            case MYSQL_TYPE_VAR_STRING:
                cols[i].size = (int) MIN(fields[i].length, 30);
                cols[i].fmt = (char *) calloc(10, sizeof(char));
                snprintf(cols[i].fmt, 10, "| %s%d.*s ", "%-",
                         cols[i].size);
                width += cols[i].size + 3;
                break;
            default:
                cols[i].fmt = strdup("| %-30.*s");
                cols[i].size = 30;
                width += cols[i].size + 3;
                break;
            }
        }
        for (i = 0; i < width; i++)
            printf("-");
        printf("\n");
        for (i = 0; i < fields_count; i++)
            printf(cols[i].fmt, cols[i].size, fields[i].name);
        printf(" |\n");
        for (i = 0; i < width; i++)
            printf("-");
        printf("\n");

        while ((row = mysql_fetch_row(res)) != NULL) {
            for (i = 0; i < fields_count; i++) {
                printf(cols[i].fmt, cols[i].size, row[i]);
            }
            printf(" |\n");
        }

        for (i = 0; i < width; i++)
            printf("-");
        printf("\n");

        mysql_free_result(res);
        for (i = 0; i < fields_count; i++)
            free(cols[i].fmt);
        free(cols);

        ret = 1;
    }

    return ret;
}

/*! \fn int db_multiple_query(const char *sql)
 *  \brief Execute multiple SQL commands
 *  \details This function is responsible to issue the SQL instructions or 
 *  call stored procedures that returns different result sets.
 *  \param query SQL instruction(s) to issue
 *  \return      Status of the operation 
 */
int
db_multiple_query(const char *sql)
{
    MYSQL_RES      *res;
    int             status;

    if (!db_conn)
        return -1;

    status = mysql_query(db_conn, sql);
    if (!status) {
        do {
            if ((res = mysql_store_result(db_conn))) {
                mysql_free_result(res);
            }
            if ((status = mysql_next_result(db_conn)) > 0) {
                fprintf(stderr,
                        "[ERR] Unable to execute the command (%s)\n\n",
                        mysql_error(db_conn));
                return -1;
            }
        } while (status == 0);
    }
    return 1;
}

/*! \fn int check_user_exist(int uid, char *full_name, int len)
 *  \brief Check if the given user ID exists
 *  \details This function checks whenever the user ID exists in the "users" 
 *  table and returns his full name if present
 *  \param uid       User ID
 *  \param full_name Buffer which will contain user's full name
 *  \param len       Buffer lenght
 *  \return          Status of the operation 
 */
int
check_user_exist(int uid, char *full_name, int len)
{
    my_ulonglong    row_count;
    MYSQL_RES      *res;
    char            buffer[1024];
    int             ret;

    if ((uid <= 0) || !db_conn)
        return -1;

    snprintf(buffer, 1024,
             "SELECT name, surname FROM users WHERE uid = %d", uid);
    ret = mysql_query(db_conn, buffer);
    if (ret != 0)
        return -1;

    res = mysql_store_result(db_conn);
    row_count = mysql_num_rows(res);
    if (row_count > 0) {
        MYSQL_ROW       row = mysql_fetch_row(res);
        if (len)
            snprintf(full_name, len, "%s %s", row[0], row[1]);
    }
    mysql_free_result(res);

    return (row_count > 0);
}

/*! \fn int check_offer_exist(int oid, char *offer_name, int len)
 *  \brief Check if the given offer ID exists
 *  \details This function checks whenever the offer ID exists in the "offers" 
 *  table and returns its name if present
 *  \param oid        Offer ID
 *  \param offer_name Buffer which will contain offer name
 *  \param len        Buffer lenght
 *  \return           Status of the operation 
 */
int
check_offer_exist(int oid, char *offer_name, int len)
{
    my_ulonglong    row_count;
    MYSQL_RES      *res;
    char            buffer[1024];
    int             ret;

    if ((oid <= 0) || !db_conn)
        return 0;

    snprintf(buffer, 1024, "SELECT name FROM offers WHERE oid = %d", oid);
    ret = mysql_query(db_conn, buffer);
    if (ret != 0)
        return 0;

    res = mysql_store_result(db_conn);
    row_count = mysql_num_rows(res);
    if (row_count > 0) {
        MYSQL_ROW       row = mysql_fetch_row(res);
        if (len)
            snprintf(offer_name, len, "%s", row[0]);
    }
    mysql_free_result(res);

    return (row_count > 0);
}

/*! \fn int check_product_exist(int pid, char *product_name, int len)
 *  \brief Check if the given product ID exists
 *  \details This function checks whenever the product ID exists in the 
 *  "product" table and returns its name if present
 *  \param oid          Product ID
 *  \param product_name Buffer which will contain product name
 *  \param len          Buffer lenght
 *  \return             Status of the operation 
 */
int
check_product_exist(int pid, char *product_name, int len, float *prezzo)
{
    my_ulonglong    row_count;
    MYSQL_RES      *res;
    char            buffer[1024];
    int             ret;

    if ((pid <= 0) || !db_conn)
        return 0;

    snprintf(buffer, 1024,
             "SELECT name, price FROM catalogue WHERE pid = %d", pid);
    ret = mysql_query(db_conn, buffer);
    if (ret != 0)
        return 0;

    res = mysql_store_result(db_conn);
    row_count = mysql_num_rows(res);
    if (row_count > 0) {
        MYSQL_ROW       row = mysql_fetch_row(res);
        if (len)
            snprintf(product_name, len, "%s", row[0]);
        if (prezzo)
            *prezzo = (float) atof(row[1]);
    }
    mysql_free_result(res);

    return (row_count > 0);
}
