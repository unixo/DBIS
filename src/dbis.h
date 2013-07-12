/*! \file dbis.h
 * \author Ferruccio Vitale (unixo@devzero.it) 
 * \date 19/01/2010 
 *
 * \note
 * Applied Computer Science - Urbino University "Carlo Bo"\n 
 * Databases and information systems\n Professor Alessandro Aldini\n 
 * Academic Year 2009 - 2010 
 */
#ifndef __DBIS_H__
#define __DBIS_H__

#include "db_util.h"

#define SQL_OP1   "SELECT pid, category, name, price, availability " \
                  "FROM catalogue ORDER BY pid, category, name"
#define SQL_OP2a  "SELECT category, name, descr, price, availability " \
                  "FROM catalogue WHERE pid = %s"
#define SQL_OP2b  "SELECT COUNT(*) AS times FROM order_details " \
                  "WHERE pid = %s"
#define SQL_OP3   "CALL offers_by_product(%d)"
#define SQL_OP4a  "SELECT * FROM users WHERE uid = %s"
#define SQL_OP4b  "SELECT oid, date, category, product, order_total " \
                  "FROM handled_orders WHERE uid = %s ORDER BY oid, date"
#define SQL_OP5a  "SELECT oid, date, customer, category, product, order_total " \
                  "FROM handled_orders ORDER BY date"
#define SQL_OP5b  "SELECT COUNT(*) AS Count, P.pid, P.name " \
                  "FROM order_details C, products P " \
                  "WHERE C.pid = P.pid GROUP BY C.pid ORDER BY Count DESC LIMIT 5"
#define SQL_OP6   "INSERT INTO categories VALUES (0, '%s')"
#define SQL_OP7   "INSERT INTO products VALUES (0, %d, '%s', '%s', %f, %d, 0)"
#define SQL_OP8   "INSERT INTO users VALUES (0, '%s', '%s', '%s'," \
                  " PASSWORD('%s'), '%s', '%s')"
#define SQL_OP9   "INSERT INTO configurations VALUES (%d, %d);"
#define SQL_OP10  "START TRANSACTION;\nINSERT INTO orders " \
                  "VALUES (0, %d, NOW(), %f);\n" \
                  "SELECT @oid := LAST_INSERT_ID();\n"
#define SQL_OP11a "SELECT DISTINCT P.pid, P.name " \
                  "FROM order_details C, products P WHERE P.pid = C.pid " \
                  "ORDER BY P.name"
#define SQL_OP11b "CALL product_delete(%s)"
#define SQL_OP12  "SELECT DATE_FORMAT(date, '%Y') AS year, " \
                  "DATE_FORMAT(date, '%m') as month, SUM(total) AS total_sales " \
                  "FROM orders GROUP BY year, month ORDER BY year, month"
#define SQL_OP13  "CALL init_database()"

/*! \var typedef void(*fn)()
 *  \brief Pointer to a function that doesn't return any value and with no 
 *  parameters 
 */
typedef void    (*fn) ();

/*
 * Functions prototype
 */
void            OP1_catalog_browsing();
void            OP2_product_detail();
void            OP3_offers_by_product();
void            OP4_user_profile_and_orders();
void            OP5_handled_orders();
void            OP6_add_category();
void            OP7_add_product();
void            OP8_register_user();
void            OP9_update_offer();
void            OP10_place_order();
void            OP11_delete_product();
void            OP12_monthly_trend();
void            OP13_init_database();

void            wait_for_user();
int             read_number(int *);
int             read_string(char *);

#endif