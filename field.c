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

SQL_FIELD *
sql_stmt_field(SQL_STATEMENT *statement, unsigned int col)
{
	return statement->api->field(statement, col);
}

int
sql_field_destroy(SQL_FIELD *field)
{
	return field->api->free(field);
}
	
const char *
sql_field_name(SQL_FIELD *field)
{
	return field->api->name(field);
}
	
size_t
sql_field_width(SQL_FIELD *field)
{
	return field->api->width(field);
}
