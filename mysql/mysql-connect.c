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

/* TODO:
 *   percent-decode components
 *   connection options
 */
int
sql_mysql_connect_(SQL *me, URI *uri)
{
	URI_INFO *info;
	MYSQL *res;
	char *pw, *db;
	unsigned long flags;

	info = uri_info(uri);
	if(!info)
	{
		return -1;
	}
	pw = NULL;
	if(info->auth)
	{
		pw = strchr(info->auth, ':');
		if(pw)
		{
			*pw = 0;
			pw++;
		}
	}
	db = NULL;
	if(info->path)
	{
		db = info->path;
		while(*db == '/')
		{
			db++;
		}
		if(!*db)
		{
			db = NULL;
		}
	}
	flags = 0;
	res = mysql_real_connect(&(me->mysql), info->host, info->auth, pw, db, info->port, NULL, flags);
	uri_info_destroy(info);
	if(!res)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	sql_mysql_execute_(me, "SET NAMES 'utf'", NULL);
	sql_mysql_execute_(me, "SET sql_mode='ANSI_QUOTES,IGNORE_SPACE,PIPES_AS_CONCAT'", NULL);
	sql_mysql_execute_(me, "SET storage_engine='InnoDB'", NULL);
	sql_mysql_execute_(me, "SET time_zone='+00:00'", NULL);
	return 0;
}

const char *
sql_mysql_sqlstate_(SQL *me)
{
	return me->sqlstate;
}

const char *
sql_mysql_error_(SQL *me)
{
	return me->error;
}

void
sql_mysql_set_error_(SQL *restrict me, const char *restrict sqlstate, const char *restrict message)
{
	strncpy(me->sqlstate, sqlstate, sizeof(me->sqlstate) - 1);
	if(!message)
	{
		message = sqlstate;
	}
	strncpy(me->error, message, sizeof(me->error) - 1);
}

void
sql_mysql_copy_error_(SQL *me)
{
	const char *sqlstate;
	const char *err;
	
	sqlstate = mysql_sqlstate(&(me->mysql));
	err = mysql_error(&(me->mysql));
	sql_mysql_set_error_(me, sqlstate, err);
}
