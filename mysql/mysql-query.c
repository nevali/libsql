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

int
sql_mysql_execute_(SQL *restrict me, const char *restrict statement, void *restrict *restrict resultdata)
{
	int r;
	MYSQL_RES *res;

	r = mysql_query(&(me->mysql), statement);
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	if(resultdata)
	{
		*resultdata = NULL;
		if(mysql_field_count(&(me->mysql)))
		{
			res = mysql_store_result(&(me->mysql));
			if(!res)
			{
				sql_mysql_copy_error_(me);
				return -1;
			}
			*resultdata = res;
		}
	}
	return 0;
}
