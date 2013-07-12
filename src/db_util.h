/*! \file db_util.h
 * \author Ferruccio Vitale (unixo@devzero.it) 
 * \date 19/01/2010 
 *
 * \note 
 * Applied Computer Science - Urbino University "Carlo Bo"\n 
 * Databases and information systems\n Professor Alessandro Aldini\n 
 * Academic Year 2009 - 2010 
 */
#ifndef __DB_UTIL_H__
#define __DB_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>

/*
 * Function prototypes
 */
void            db_connect();
void            db_close();
my_ulonglong    db_last_id();
int             db_query_with_parameter(char *, char *, int);
int             db_multiple_query(const char *);
int             check_user_exist(int, char *, int);
int             check_offer_exist(int, char *, int);
int             check_product_exist(int, char *, int, float *);

#endif                          /* __DB_UTIL_H__ */
