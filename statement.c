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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_libsql.h"


/* Execute a statement not expected to return a result-set */
int
sql_execute(SQL *restrict sql, const char *restrict statement)
{
	int r;

	r = sql->api->execute(sql, statement, NULL);

	return r;
}

/* Execute a statement which is expected to return a result-set */
SQL_STATEMENT *
sql_query(SQL *restrict sql, const char *restrict statement)
{
	SQL_STATEMENT *rs;
	void *data;
	int r;

	data = NULL;
	rs = sql->api->statement(sql, NULL);
	if(!rs)
	{
		return NULL;
	}
	r = sql->api->execute(sql, statement, &data);
	if(r)
	{
		rs->api->release(rs);
		return NULL;
	}
	rs->api->set_results(rs, data);
	return rs;
}

int
sql_stmt_destroy(SQL_STATEMENT *stmt)
{
	return stmt->api->release(stmt);
}

unsigned int
sql_stmt_columns(SQL_STATEMENT *stmt)
{
	return stmt->api->columns(stmt);
}

unsigned long long
sql_stmt_rows(SQL_STATEMENT *stmt)
{
	return stmt->api->rows(stmt);
}

unsigned long long
sql_stmt_affected(SQL_STATEMENT *stmt)
{
	return stmt->api->affected(stmt);
}

int
sql_stmt_eof(SQL_STATEMENT *stmt)
{
	return stmt->api->eof(stmt);
}

int
sql_stmt_next(SQL_STATEMENT *stmt)
{
	return stmt->api->next(stmt);
}

int
sql_stmt_null(SQL_STATEMENT *stmt, unsigned int col)
{
	return stmt->api->null(stmt, col);
}

size_t
sql_stmt_value(SQL_STATEMENT *restrict stmt, unsigned int col, char *restrict buf, size_t buflen)
{
	return stmt->api->value(stmt, col, buf, buflen);
}
	