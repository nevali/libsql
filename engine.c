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

SQL_ENGINE *sql_mysql_engine(void);


SQL_ENGINE *
sql_engine_(URI *uri)
{
	char scheme[64];
	size_t r;

	r = uri_scheme(uri, scheme, sizeof(scheme));
	if(r == (size_t) -1)
	{
		sql_set_error_("53000", "Failed to obtain scheme from parsed URI");
		return NULL;
	}
	if(r >= sizeof(scheme))
	{
		sql_set_error_("08000", "The specified URI scheme is not supported by any client engine");
		return NULL;
	}
	if(!strcmp(scheme, "mysql") || !strcmp(scheme, "mysqls"))
	{
		return sql_mysql_engine();
	}
	sql_set_error_("08000", "The specified URI scheme is not supported by any client engine");
	return NULL;
}

