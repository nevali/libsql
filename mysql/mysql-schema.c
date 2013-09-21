/*
 * Copyright 2013 Mo McRoberts.
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

/* All methods below are invoked within a transaction by sql_migrate() */

#define SCHEMA_SQL \
	"CREATE TABLE IF NOT EXISTS \"_version\" (" \
	"\"ident\" VARCHAR(64) NOT NULL COMMENT 'Module identifier', " \
	"\"version\" BIGINT UNSIGNED NOT NULL COMMENT 'Current schema version'," \
	"\"updated\" DATETIME NOT NULL COMMENT 'Timestamp of the last schema update'," \
	"\"comment\" TEXT DEFAULT NULL COMMENT 'Description of the last update'," \
	"PRIMARY KEY (\"ident\")" \
	") ENGINE=InnoDB DEFAULT CHARSET=utf8 DEFAULT COLLATE=utf8_unicode_ci"

static int sql_mysql_schema_select_version_(SQL *me, const char *identifier, size_t idlen, int defver);

int
sql_mysql_schema_get_version_(SQL *me, const char *identifier)
{
	size_t idlen, qbuflen;
	char *p;
	
	idlen = strlen(identifier);
	if(idlen > 64)
	{
		idlen = 64;
	}
	qbuflen = (idlen * 2) + 128;
	if(qbuflen > me->qbuflen)
	{
		p = (char *) realloc(me->qbuf, qbuflen);
		if(!p)
		{
			return -1;
		}
		me->qbuf = p;
		me->qbuflen = qbuflen;
	}
	return sql_mysql_schema_select_version_(me, identifier, idlen, 0);
}

int
sql_mysql_schema_create_table_(SQL *me)
{
	int r;
	
	if(me->querylog)
	{
		me->querylog(me, SCHEMA_SQL);
	}
	r = mysql_query(&(me->mysql), SCHEMA_SQL);
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	return 0;
}

int
sql_mysql_schema_set_version_(SQL *me, const char *identifier, int version)
{
	size_t idlen;
	char *p;
	
	if(version < 0)
	{
		return version;
	}
	idlen = strlen(identifier);
	if(idlen > 64)
	{
		idlen = 64;
	}
	if(sql_mysql_schema_select_version_(me, identifier, idlen, -1) < 0)
	{
		strcpy(me->qbuf, "INSERT INTO \"_version\" (\"ident\", \"version\", \"updated\") VALUES ('");
		p = strchr(me->qbuf, 0);
		mysql_real_escape_string(&(me->mysql), p, identifier, idlen);
		p = strchr(me->qbuf, 0);
		sprintf(p, "', %d, NOW())", version);
	}
	else
	{
		snprintf(me->qbuf, me->qbuflen, "UPDATE \"_version\" SET \"version\" = %d, \"updated\" = NOW() WHERE \"ident\" = '", version);
		p = strchr(me->qbuf, 0);
		mysql_real_escape_string(&(me->mysql), p, identifier, idlen);
		p = strchr(me->qbuf, 0);
		*p = '\'';
		p++;
		*p = 0;
	}
	if(me->querylog)
	{
		me->querylog(me, me->qbuf);
	}
	if(mysql_query(&(me->mysql), me->qbuf))
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	return version;
}

static int
sql_mysql_schema_select_version_(SQL *me, const char *identifier, size_t idlen, int defver)
{
	char *p;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int r;
	
	strcpy(me->qbuf, "SELECT \"version\" FROM \"_version\" WHERE \"ident\" = '");
	p = strchr(me->qbuf, 0);
	mysql_real_escape_string(&(me->mysql), p, identifier, idlen);
	p = strchr(me->qbuf, 0);
	*p = '\'';
	p++;
	*p = 0;
	if(me->querylog)
	{
		me->querylog(me, me->qbuf);
	}
	r = mysql_query(&(me->mysql), me->qbuf);
	if(r)
	{
		sql_mysql_copy_error_(me);
		return -1;		
	}
	res = mysql_store_result(&(me->mysql));
	if(!res)
	{
		sql_mysql_copy_error_(me);
		return -1;
	}
	row = mysql_fetch_row(res);
	if(!row || !row[0])
	{
		mysql_free_result(res);
		return defver;
	}
	r = atoi(row[0]);
	mysql_free_result(res);
	return r;	
}
