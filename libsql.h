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

#ifndef LIBSQL_H_
# define LIBSQL_H_                      1

# include <stdarg.h>
# include <liburi.h>

typedef struct sql_struct SQL;
typedef struct sql_statement_struct SQL_STATEMENT;
typedef struct sql_field_struct SQL_FIELD; 

# if (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L) && !defined(restrict)
#  define restrict
# endif

# if defined(__cplusplus)
extern "C" {
# endif
  
	SQL *sql_connect(const char *uri);
	SQL *sql_connect_uri(URI *uri);
	int sql_disconnect(SQL *sql);

	const char *sql_sqlstate(SQL *connection);
	const char *sql_error(SQL *connection);

	/* Execute a statement not expected to return a result-set */
	int sql_execute(SQL *restrict sql, const char *restrict statement);
	int sql_executef(SQL *restrict sql, const char *restrict statement, ...);
	int sql_vexecutef(SQL *restrict sql, const char *restrict statement, va_list ap);

	/* Execute a statement which is expected to return a result-set */
	SQL_STATEMENT *sql_query(SQL *restrict sql, const char *restrict statement);
	SQL_STATEMENT *sql_queryf(SQL *restrict sql, const char *restrict statement, ...);
	SQL_STATEMENT *sql_vqueryf(SQL *restrict sql, const char *restrict statement, va_list ap);

	/* Create a parameterised statement */
	SQL_STATEMENT *sql_stmt_create(const char *statement);
	
	/* Execute a parameterised statement */
	int sql_stmt_execf(SQL_STATEMENT *statement, ...);
	int sql_stmt_vexecf(SQL_STATEMENT *statement, va_list ap);
	
	/* Destroy a statement or result-set */
	int sql_stmt_destroy(SQL_STATEMENT *statement);
	
	int sql_stmt_next(SQL_STATEMENT *statement);
	int sql_stmt_eof(SQL_STATEMENT *statement);
	int sql_stmt_rewind(SQL_STATEMENT *statement);
	int sql_stmt_seek(SQL_STATEMENT *statement, unsigned long long row);
	unsigned int sql_stmt_columns(SQL_STATEMENT *statement);
	unsigned long long sql_stmt_rows(SQL_STATEMENT *statement);
	unsigned long long sql_stmt_affected(SQL_STATEMENT *statement);
	unsigned long long sql_stmt_cur(SQL_STATEMENT *statement);
	SQL_FIELD *sql_stmt_field(SQL_STATEMENT *statement, unsigned int col);	
	size_t sql_stmt_value(SQL_STATEMENT *restrict statement, unsigned int col, char *restrict buf, size_t buflen);
	
	/* Return the name of a column */
	int sql_field_destroy(SQL_FIELD *field);
	const char *sql_field_name(SQL_FIELD *field);
	size_t sql_field_width(SQL_FIELD *field);
	
# if defined(__cplusplus)
}
# endif

#endif /*!LIBSQL_H_*/
