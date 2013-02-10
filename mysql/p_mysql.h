/*
 * Copyright 2012-2013 Mo McRoberts.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef P_MYSQL_H_
# define P_MYSQL_H_                     1

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <libsql.h>
# include <mysql.h>

# define SQL_STRUCT_DEFINED             1

# include <libsql-engine.h>

struct sql_engine_struct
{
	SQL_ENGINE_API *api;
};

struct sql_struct
{
	SQL_API *api;
	MYSQL mysql;
	char sqlstate[6];
	char error[512];
};

struct sql_statement_struct
{
	SQL_STATEMENT_API *api;
	SQL *sql;
	char *statement;	
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *fields;
	unsigned long *lengths;
	unsigned int columns;
	unsigned long long affected;
	unsigned long long rows;
	unsigned long long cur;
};

struct sql_field_struct
{
	SQL_FIELD_API *api;
	MYSQL_FIELD *field;
};

SQL *sql_engine_mysql_create_(SQL_ENGINE *me);

void sql_mysql_set_error_(SQL *restrict me, const char *restrict sqlstate, const char *restrict message);
void sql_mysql_copy_error_(SQL *me);

int sql_mysql_free_(SQL *me);
const char *sql_mysql_sqlstate_(SQL *me);
const char *sql_mysql_error_(SQL *me);
int sql_mysql_connect_(SQL *restrict me, URI *restrict uri);
int sql_mysql_execute_(SQL *restrict me, const char *restrict statement, void *restrict *restrict resultdata);
SQL_STATEMENT *sql_mysql_statement_(SQL *restrict me, const char *restrict statement);

int sql_statement_mysql_free_(SQL_STATEMENT *me);
const char *sql_statement_mysql_statement_(SQL_STATEMENT *me);
int sql_statement_mysql_set_results_(SQL_STATEMENT *restrict me, void *data);
unsigned int sql_statement_mysql_columns_(SQL_STATEMENT *restrict me);
unsigned long long sql_statement_mysql_rows_(SQL_STATEMENT *me);
unsigned long long sql_statement_mysql_affected_(SQL_STATEMENT *me);
int sql_statement_mysql_eof_(SQL_STATEMENT *me);
int sql_statement_mysql_next_(SQL_STATEMENT *me);
SQL_FIELD *sql_statement_mysql_field_(SQL_STATEMENT *me, unsigned int col);
size_t sql_statement_mysql_value_(SQL_STATEMENT *restrict me, unsigned int col, char *restrict buf, size_t buflen);
size_t sql_statement_mysql_valuelen_(SQL_STATEMENT *me, unsigned int col);
unsigned long long sql_statement_mysql_cur_(SQL_STATEMENT *me);
int sql_statement_mysql_seek_(SQL_STATEMENT *me, unsigned long long row);
int sql_statement_mysql_rewind_(SQL_STATEMENT *me);

int sql_field_mysql_free_(SQL_FIELD *me);
const char *sql_field_mysql_name_(SQL_FIELD *me);
size_t sql_field_mysql_width_(SQL_FIELD *me);

#endif /*!P_MYSQL_H_*/
