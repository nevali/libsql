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

static SQL_FIELD_API mysql_field_api = {
	NULL,
	NULL,
	sql_field_mysql_free_,
	sql_field_mysql_name_,
	sql_field_mysql_width_
};

/* Return a SQL_FIELD instance for a particular column (0..cols-1) */
SQL_FIELD *
sql_statement_mysql_field_(SQL_STATEMENT *me, unsigned int col)
{
	SQL_FIELD *p;
	
	if(!me->fields || col >= me->columns)
	{
		return NULL;
	}
	p = (SQL_FIELD *) calloc(1, sizeof(SQL_FIELD));
	p->api = &mysql_field_api;
	p->field = &(me->fields[col]);
	return p;
}

/* Free a SQL_FIELD structure */
int
sql_field_mysql_free_(SQL_FIELD *me)
{
	free(me);
	return 0;
}

/* Return a pointer to the name of the column */
const char *
sql_field_mysql_name_(SQL_FIELD *me)
{
	return me->field->name;
}

/* Return the maximum width (i.e., number of characters) for this column */
size_t
sql_field_mysql_width_(SQL_FIELD *me)
{
	return me->field->max_length;
}
