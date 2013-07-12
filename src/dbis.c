/*! \file dbis.c
 * \author Ferruccio Vitale (unixo@devzero.it) 
 * \date 19/01/2010 
 *
 * \note 
 * Applied Computer Science - Urbino University "Carlo Bo"\n 
 * Databases and information systems\n Professor Alessandro Aldini\n 
 * Academic Year 2009 - 2010 
 */
#include "dbis.h"

extern MYSQL   *db_conn;

fn  operations[] = {
    OP1_catalog_browsing,
    OP2_product_detail,
    OP3_offers_by_product,
    OP4_user_profile_and_orders,
    OP5_handled_orders,
    OP6_add_category,
    OP7_add_product,
    OP8_register_user,
    OP9_update_offer,
    OP10_place_order,
    OP11_delete_product,
    OP12_monthly_trend,
    OP13_init_database
};

/*! \fn void OP1_catalog_browsing() 
 *  \brief Display product catalog 
 */
void
OP1_catalog_browsing()
{
    printf("\nPRODUCT CATALOG\n");
    db_query_with_parameter(SQL_OP1, NULL, 1);
    printf("\n\n");
    wait_for_user();
}

/*! \fn void OP2_product_detail()
 *  \brief Given a product ID, the function displays its details 
 */
void
OP2_product_detail()
{
    int             pid;
    char            s_pid[10];

    printf("\nVIEWING PRODUCT DETAIL\n" "Product ID [0 to exit]: ");
    if (read_number(&pid) == 0)
        return;
    if (pid == 0) {
        fprintf(stderr, "Operation aborted\n");
    } else {
        snprintf(s_pid, 10, "%d", pid);
        db_query_with_parameter(SQL_OP2a, s_pid, 1);
        printf("\nThe product has been sold (# times):\n");
        db_query_with_parameter(SQL_OP2b, s_pid, 1);
        printf("\n\n");
    }
    wait_for_user();
}

/*! \fn void OP3_offers_by_product()
 *  \brief Given a product ID, it displays all offers that contains it 
 */
void
OP3_offers_by_product()
{
    MYSQL_RES      *result;
    MYSQL_ROW       row;
    int             pid,
                    ret;
    char            buffer[1024];

    printf("\nVIEW OFFERS BY PRODUCT\n"
           "Select a product ID [0 to abort]: ");
    if ((read_number(&pid) == 0) || (pid == 0)) {
        fprintf(stderr, "Operation aborted\n\n");
        wait_for_user();
        return;
    }
    snprintf(buffer, 1024, SQL_OP3, pid);
    if ((ret = mysql_query(db_conn, buffer)) != 0) {
        fprintf(stderr, "Error during stored procedure execution.\n");
        wait_for_user();
        return;
    }

    do {
        if ((result = mysql_store_result(db_conn))) {
            /*
             * Evaluate the result set; at the end I'll free memory
             */
            int i, fields_count = mysql_num_fields(result);

            if (fields_count == 2) {
                for (i = 0; i < 67; i++)
                    printf("-");
                printf("\n");
                row = mysql_fetch_row(result);
                printf("| %-48s PRICE   %6s |\n", row[0], row[1]);
            } else {
                for (i = 0; i < 67; i++)
                    printf("-");
                printf("\n");
                while ((row = mysql_fetch_row(result))) {
                    printf("| %-20s | %-25s | %6s | %3s | ", row[7],
                           row[2], row[4], row[5]);
                    printf("%s", (pid == atoi(row[0])) ? "<<--\n" : "\n");
                }
                for (i = 0; i < 67; i++)
                    printf("-");
                printf("\n\n");
            }
            mysql_free_result(result);
        }
        /*
         * Check if there other result set waiting to be fetched
         */
        if ((ret = mysql_next_result(db_conn)) > 0)
            printf("[ERR] unable to issue the command.\n");
    } while (ret == 0);
    printf("\n\n");
    wait_for_user();
}

/*! \fn void OP4_user_profile_and_orders()
 *  \brief Display user profile and all his orders 
 */
void
OP4_user_profile_and_orders()
{
    int             uid;
    char            s_uid[10];

    printf("\nVIEW USER PROFILE AND ORDERS\n"
           "Customer ID [0 to abort]: ");
    if ((read_number(&uid) == 0) || (uid == 0)) {
        fprintf(stderr, "[ERR] Operation aborted\n\n");
        wait_for_user();
        return;
    }
    snprintf(s_uid, 10, "%d", uid);
    printf("CUSTOMER DETAILS\n");
    db_query_with_parameter(SQL_OP4a, s_uid, 1);
    printf("\nCUSTOMER ORDERS\n");
    db_query_with_parameter(SQL_OP4b, s_uid, 1);
    printf("\n");
    wait_for_user();
}

/*! \fn void OP5_handled_orders() 
 *  \brief Display all handled orders 
 */
void
OP5_handled_orders()
{
    printf("\nVIEW HANDLED ORDERS\n");
    db_query_with_parameter(SQL_OP5a, NULL, 1);
    printf("\nTOP 5 PRODUCTS MOSTLY SOLD\n");
    db_query_with_parameter(SQL_OP5b, NULL, 1);
    printf("\n");
    wait_for_user();
}


/*! \fn void OP6_add_category()
 *  \brief Add a new category of product 
 */
void
OP6_add_category()
{
    char            buffer[46];

    printf("\nADD NEW CATEGORY\nCategory name [. to abort]: ");
    read_string(buffer);
    if (strcmp(buffer, ".") == 0) {
        fprintf(stderr, "Operation aborted.\n\n");
    } else {
        if (db_query_with_parameter(SQL_OP6, buffer, 0) == 0) {
            printf("Category successfully created [ID=%lu]\n\n",
                   (unsigned long) db_last_id());
        }
    }
    wait_for_user();
}


/*! \fn void OP7_add_product()
 *  \brief Add a new product for sale 
 */
void
OP7_add_product()
{
    int             cid;
    char            name[45],
                    descr[255],
                    buffer[1024];
    float           price;
    int             availability, ret;

    printf("\nAVAILABLE CATEGORIES:\n");
    db_query_with_parameter("SELECT * FROM categories ORDER BY cid", NULL,
                            1);
    printf("Choose a category [0 to abort]: ");
    read_number(&cid);
    if (cid == 0) {
        printf("Operation aborted.\n\n");
        wait_for_user();
        return;
    }
    printf("Name         : ");
    read_string(name);
    printf("Description  : ");
    read_string(descr);
    printf("Price        : ");
    ret = fscanf(stdin, "%f", &price);
    printf("Availability : ");
    read_number(&availability);
    snprintf(buffer, 1024, SQL_OP7, cid, name, descr, price, availability);
    if (db_query_with_parameter(buffer, NULL, 0) == 0) {
        printf("Product added with success [ID=%lu]\n\n",
               (unsigned long) db_last_id());
    }
    wait_for_user();
}

/*! \fn void OP8_register_user()
 *  \brief Register a new user 
 */
void
OP8_register_user()
{
    char            name[20],
                    surname[20],
                    login[15],
                    passwd[45];
    char            address[45],
                    city[15],
                    buffer[255];

    printf("\nREGISTER NEW USER\n");
    printf("Name    : ");
    read_string(name);
    printf("Surname : ");
    read_string(surname);
    printf("Login   : ");
    read_string(login);
    printf("Password: ");
    read_string(passwd);
    printf("Address : ");
    read_string(address);
    printf("City    : ");
    read_string(city);

    snprintf(buffer, 255, SQL_OP8, name, surname, login, passwd, address,
             city);
    if (db_query_with_parameter(buffer, NULL, 0) == 0) {
        printf("User successfully registered [ID=%lu]\n",
               (unsigned long) db_last_id());
    }
    wait_for_user();
}

/*! \fn void OP9_update_offer()
 *  \brief Add one o more products to an existing offer 
 */
void
OP9_update_offer()
{
    int             oid,
                    pid,
                    pid_count,
                   *pid_array,
                    error,
                    i;
    char            product_name[128],
                    offer_name[128],
                    buffer[256];
    float           price;

    printf("\nUPDATE OFFER\nEnter offer ID [0 to abort]: ");
    if ((read_number(&oid) == 0) || (oid == 0)) {
        printf("Operation aborted\n\n");
        wait_for_user();
        return;
    }
    if (check_offer_exist(oid, offer_name, 128) != 1) {
        printf("[ERR] not existent offer\n\n");
        wait_for_user();
        return;
    }
    printf("Selected offer: %s\n", offer_name);
    pid_count = 0;
    pid_array = NULL;
    do {
        printf("Enter product ID to add to the offer [0 to abort]: ");
        if (read_number(&pid)) {
            if (pid != 0) {
                if (check_product_exist(pid, product_name, 128, &price) !=
                    1) {
                    fprintf(stderr, "[ERR] not existent product.\n");
                } else {
                    error = 0;
                    if (pid_array) {
                        for (i = 0; i < pid_count; i++) {
                            if (pid_array[i] == pid) {
                                fprintf(stderr,
                                        "Product already selected.\n");
                                error = 1;
                                continue;
                            }
                        }
                        if (!error) {
                            pid_count++;
                            pid_array =
                                (int *) realloc(pid_array, pid_count);
                        }
                    } else {
                        pid_count = 1;
                        pid_array = (int *) calloc(pid_count, sizeof(int));
                    }
                    if (!error) {
                        printf("+ Product added: %s (%f)\n", product_name,
                               price);
                        pid_array[pid_count - 1] = pid;
                    }
                }
            }
        }
    } while (pid != 0);
    if (pid_count == 0) {
        printf("No products have been selected.\n\n");
        wait_for_user();
        return;
    }
    for (i = 0; i < pid_count; i++) {
        snprintf(buffer, 256, SQL_OP9, oid, pid_array[i]);
        db_query_with_parameter(buffer, NULL, 0);
    }
    free(pid_array);
    printf("\n\n");
    wait_for_user();
}

/*! \fn void OP10_place_order()
 *  \brief Place a new order by a customer 
 */
void
OP10_place_order()
{
    int             uid,
                    pid;
    int            *pid_array = NULL,
        pid_count = 0, ret,
        i,
        error;
    char            var[10],
                    full_name[128],
                    product_name[128];
    char            buffer[256],
                    sql[2048];
    float           price,
                    total = 0,
        discount;

    printf("\nPLACE A NEW ORDER\nCustomer ID [0 to abort]: ");
    if ((read_number(&uid) == 0) || (uid == 0)) {
        printf("Operation aborted\n\n");
        wait_for_user();
        return;
    }
    if (check_user_exist(uid, full_name, 128) <= 0) {
        fprintf(stderr, "User doesn't exist.\n");
        wait_for_user();
        return;
    }
    printf("Selected user: %s\n\n", full_name);
    do {
        do {
            printf("Enter product ID [0 to abort, ? to display list]: ");
            read_string(var);
            if (strcmp(var, "?") == 0) {
                OP1_catalog_browsing();
                pid = -1;
            } else
                pid = atoi(var);
        } while (pid == -1);
        if (pid != 0) {
            if (check_product_exist(pid, product_name, 128, &price) <= 0) {
                fprintf(stderr, "This product doesn't exist.\n");
            } else {
                error = 0;
                if (pid_array) {
                    for (i = 0; i < pid_count; i++) {
                        if (pid_array[i] == pid) {
                            fprintf(stderr, "Product already selected.\n");
                            error = 1;
                            continue;
                        }
                    }
                    if (!error) {
                        pid_count++;
                        pid_array = (int *) realloc(pid_array, pid_count);
                    }
                } else {
                    pid_count = 1;
                    pid_array = (int *) calloc(pid_count, sizeof(int));
                }
                if (!error) {
                    printf("+ Selected product: %s\n", product_name);
                    pid_array[pid_count - 1] = pid;
                    total += price;
                }
            }
        }
    } while (pid > 0);
    if (pid_count == 0) {
        fprintf(stderr, "No products have been added to cart.\n");
        wait_for_user();
        return;
    }
    printf("Enter the discount percentage [0 no discount]: ");
    ret = fscanf(stdin, "%f", &discount);
    if (discount != 0) {
        total -= ((total * discount) / 100);
        printf("Final price: %f\n", (float) total);
    }

    memset(sql, 0, 2048);
    snprintf(sql, 2048, SQL_OP10, uid, total);
    for (i = 0; i < pid_count; i++) {
        snprintf(buffer, 256, "INSERT INTO order_details "
                 "VALUES (@oid, %d, 1);\n", pid_array[i]);
        strcat(sql, buffer);
    }
    strcat(sql, "COMMIT;\n");
    free(pid_array);

    db_multiple_query(sql);
    printf("\n");
    wait_for_user();
}

/*! \fn void OP11_delete_product()
 *  \brief Delete a product (even if already sold) 
 */
void
OP11_delete_product()
{
    char            s_pid[10];
    int             pid;

    printf("DELETE A PRODUCT\n");
    db_query_with_parameter(SQL_OP11a, NULL, 1);
    printf("Enter product ID [0 to abort]: ");
    if ((read_number(&pid) == 0) || (pid == 0)) {
        printf("Operation aborted.\n\n");
        wait_for_user();
        return;
    }
    snprintf(s_pid, 10, "%d", pid);
    db_query_with_parameter(SQL_OP11b, s_pid, 0);
    printf("\n");
    wait_for_user();
}

/*! \fn void OP12_monthly_trend()
 *  \brief Display monthly trend of sales 
 */
void
OP12_monthly_trend()
{
    printf("\nTREND MONTHLY SALES\n");
    db_query_with_parameter(SQL_OP12, NULL, 1);
    printf("\n\n");
    wait_for_user();
}

/*! \fn void OP13_init_database()
 *  \brief Database initialization with default values 
 */
void
OP13_init_database()
{
    if (db_multiple_query(SQL_OP13) == 1)
        printf("Database initialized.\n\n");
    wait_for_user();
}

/*! \fn int read_string(int *buffer)
 *  \brief Read a string from stdin
 *  \param number Pointer to the string to be returned
 *  \return       1 if successful, 0 otherwise 
 */
int
read_string(char *buffer)
{
	int ret;

	if (!buffer)
		return 0;

	ret = fscanf(stdin, "%s", buffer);
	return (ret != 0);
}

/*! \fn int read_number(int *number)
 *  \brief Read a number from stdin
 *  \param number Pointer to the number to be returned
 *  \return       1 if successful, 0 otherwise 
 */
int
read_number(int *number)
{
    char            buffer[32];
    int             num;

    if (!fgets(buffer, 32, stdin))
        return 0;
    if (sscanf(buffer, "%d", &num) != 1)
        return 0;
    if (number)
        *number = num;

    return 1;
}

void
wait_for_user()
{
    printf("Press ENTER to continue.");
    getchar();
}

/*! \fn void menu()
 *  \brief Displays the menu of available operations to the user 
 */
void
menu()
{
    int             choice = -1, ret;

    while (choice != 0) {
        ret = system("clear");
        fprintf(stdout, "AVAILABLE OPERATIONS\n\n");
        fprintf(stdout, "( 1)  Browse product catalog by category\n"
                "( 2)  View details of a chosen product\n"
                "( 3)  Browse all the configurations which include a given item\n"
                "( 4)  Browse user profile and all his order\n"
                "( 5)  Browse all orders\n"
                "( 6)  Add a new product category\n"
                "( 7)  Add a new product\n"
                "( 8)  Register a new user\n"
                "( 9)  Update of product configuration\n"
                "(10)  Place a new order\n"
                "(11)  Delete a product (even if already sold)\n"
                "(12)  Trend monthly sales\n"
                "(13)  Initialize database\n\n" "( 0)  EXIT\n\n");
        fprintf(stdout, "Selection [0-13]: ");
        if (read_number(&choice) == 0) {
            choice = 99;
        } else if ((choice > 0) && (choice < 14)) {
            fn              op = operations[choice - 1];
            if (op)
                (*op) ();
        }
    }
}

int
main(int argc, char **argv)
{
    db_connect();
    menu();
    db_close();

    return 1;
}
