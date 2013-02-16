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

#include "p_mysql.h"

static SQL_STATEMENT_API mysql_statement_api = {
	NULL,
	NULL,
	sql_statement_mysql_free_,
	sql_statement_mysql_statement_,
	sql_statement_mysql_set_results_,
	sql_statement_mysql_columns_,
	sql_statement_mysql_rows_,
	sql_statement_mysql_affected_,
	sql_statement_mysql_field_,
	sql_statement_mysql_null_,
	sql_statement_mysql_value_,
	sql_statement_mysql_valueptr_,
	sql_statement_mysql_valuelen_,
	sql_statement_mysql_eof_,
	sql_statement_mysql_next_,
	sql_statement_mysql_cur_,
	sql_statement_mysql_rewind_,
	sql_statement_mysql_seek_
};

/* Create a new statement or result-set */
SQL_STATEMENT *
sql_mysql_statement_(SQL *restrict me, const char *restrict statement)
{
	SQL_STATEMENT *p;

	p = (SQL_STATEMENT *) calloc(1, sizeof(SQL_STATEMENT));
	if(!p)
	{
		return NULL;
	}
	p->api = &mysql_statement_api;
	p->refcount = 1;
	p->sql = me;
	p->cur = (unsigned long long) -1;
	if(statement)
	{
		p->statement = strdup(statement);
		if(!p->statement)
		{
			free(p);
			return NULL;
		}
	}
	return p;
}

/* Free the resources used by a statement */
unsigned long
sql_statement_mysql_free_(SQL_STATEMENT *me)
{
	me->refcount--;
	if(me->refcount)
	{
		return me->refcount;
	}
	if(me->result)
	{
		mysql_free_result(me->result);
	}
	free(me->statement);
	free(me);
	return 0;
}

/* Return the (parameterised) SQL statement */
const char *
sql_statement_mysql_statement_(SQL_STATEMENT *me)
{
	return me->statement;
}

/* Update the result-set with a new MYSQL_RES pointer */
int
sql_statement_mysql_set_results_(SQL_STATEMENT *restrict me, void *data)
{
	if(me->result)
	{
		mysql_free_result(me->result);
	}
	me->result = (MYSQL_RES *) data;
	me->affected = mysql_affected_rows(&(me->sql->mysql));
	me->lengths = NULL;
	me->cur = (unsigned long long) -1;
	if(data)
	{
		me->fields = mysql_fetch_fields(me->result);
		me->columns = mysql_field_count(&(me->sql->mysql));
		me->rows = mysql_num_rows(me->result);
		me->row = mysql_fetch_row(me->result);
		if(me->row)
		{
			me->lengths = mysql_fetch_lengths(me->result);
			me->cur = 0;
		}
	}
	else
	{
		me->fields = NULL;
		me->columns = 0;
		me->row = NULL;
		me->rows = 0;
	}
	return 0;
}

/* Return the number of columns in the result-set */
unsigned int
sql_statement_mysql_columns_(SQL_STATEMENT *me)
{
	return me->columns;
}

/* Return the number of rows in the result-set */
unsigned long long
sql_statement_mysql_rows_(SQL_STATEMENT *me)
{
	return me->rows;
}

/* Return the number of rows affected by the query */
unsigned long long
sql_statement_mysql_affected_(SQL_STATEMENT *me)
{
	return me->affected;
}

/* Return 1 if we have reached the end of the result-set, or 0 otherwise */
int
sql_statement_mysql_eof_(SQL_STATEMENT *me)
{
	if(me->row)
	{
		return 0;
	}
	return 1;
}

/* Move to the next row; returns 1 if there was a row, 0 if not or -1 if an error occurred */
int
sql_statement_mysql_next_(SQL_STATEMENT *me)
{
	if(!me->result)
	{
		return 0;
	}
	me->row = mysql_fetch_row(me->result);
	me->lengths = NULL;
	if(me->row)
	{
		me->lengths = mysql_fetch_lengths(me->result);
		me->cur++;
		return 1;
	}
	me->cur = (unsigned long long) -1;
	return 0;
}

/* Retrieve the contents of a column in the current row */
size_t
sql_statement_mysql_value_(SQL_STATEMENT *restrict me, unsigned int col, char *restrict buf, size_t buflen)
{
	size_t l;
	
	if(buf)
	{
		*buf = 0;
	}
	if(!me->row || col >= me->columns)
	{
		return 0;
	}
	if(buf)
	{
		l = me->lengths[col];
		if(l >= buflen)
		{
			l = buflen;
		}
		memcpy(buf, me->row[col], l);
		buf[l] = 0;
	}
	return me->lengths[col] + 1;
}

/* Retrieve a pointer to the first byte in a field, or NULL if the value is NULL */
const unsigned char *
sql_statement_mysql_valueptr_(SQL_STATEMENT *me, unsigned int col)
{
	if(!me->row || col >= me->columns)
	{
		return NULL;
	}
	return (const unsigned char *) me->row[col];
}


/* Return 1 if the specified column in the current row is NULL */
int
sql_statement_mysql_null_(SQL_STATEMENT *me, unsigned int col)
{
	if(!me->row || col >= me->columns)
	{
		return 1;
	}
	if(!me->row[col])
	{
		return 1;
	}
	return 0;
}

/* Retrieve the length of the data in a column in the current row, including terminating NULL */
size_t
sql_statement_mysql_valuelen_(SQL_STATEMENT *me, unsigned int col)
{
	if(!me->row || col >= me->columns)
	{
		return 0;
	}
	return me->lengths[col] + 1;
}

/* Return the row index of the current row */
unsigned long long
sql_statement_mysql_cur_(SQL_STATEMENT *me)
{
	return me->cur;
}

/* Seek to a particular row in the result-set */
int
sql_statement_mysql_seek_(SQL_STATEMENT *me, unsigned long long row)
{
	if(!me->result || row > me->rows)
	{
		return -1;
	}
	mysql_data_seek(me->result, row);
	me->row = mysql_fetch_row(me->result);
	me->lengths = NULL;
	if(me->row)
	{
		me->lengths = mysql_fetch_lengths(me->result);
		me->cur = row;
		return 0;
	}
	me->cur = (unsigned long long) -1;
	return -1;
}

/* Rewind the current pointer into the result-set back to the start */
int
sql_statement_mysql_rewind_(SQL_STATEMENT *me)
{
	return sql_statement_mysql_seek_(me, 0);
}
