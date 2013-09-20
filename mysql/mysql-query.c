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

	if(me->depth && me->deadlocked)
	{
		/* If we're already deadlocked mid-transaction, there's no point in
		 * doing anything
		 */
		return -1;
	}
	me->deadlocked = 0;
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

int
sql_mysql_begin_(SQL *me)
{
	int r;
	
	if(me->depth)	
	{
		/* Can't nest transactions */
		sql_mysql_set_error_(me, "25000", "You are not allowed to execute this command in a transaction");
		return -1;
	}
	r = mysql_query(&(me->mysql), "START TRANSACTION");
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	me->depth++;
	return 0;
}

int
sql_mysql_commit_(SQL *me)
{
	int r;
	
	if(!me->depth)
	{
		return 0;
	}
	if(me->deadlocked)
	{
		return -1;
	}
	r = mysql_query(&(me->mysql), "COMMIT");
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	me->depth--;
	return 0;
}

int
sql_mysql_rollback_(SQL *me)	
{
	int r;
	
	if(!me->depth)
	{
		return 0;
	}
	r = mysql_query(&(me->mysql), "ROLLBACK");
	if(me->deadlocked)
	{
		/* It doesn't matter if the rollback failed */
		me->depth--;
		me->deadlocked = 0;
		return 0;
	}
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	me->depth--;
	return 0;
}

int
sql_mysql_deadlocked_(SQL *me)
{
	return me->deadlocked;
}
