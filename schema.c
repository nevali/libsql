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

struct migrate_data
{
	int target;
	int current;
	const char *identifier;
	SQL_PERFORM_MIGRATE fn;
	void *userdata;
};

static int sql_migrate_txn_(SQL *sql, void *userdata);

int
sql_migrate(SQL *restrict sql, const char *restrict identifier, SQL_PERFORM_MIGRATE fn, void *userdata)
{
	struct migrate_data data;
	int prev;
	
	data.target = fn(sql, identifier, 0, userdata);
	if(data.target <= 0)
	{
		/* No migration to be performed */
		return data.target;
	}
	data.current = 0;
	data.identifier = identifier;
	data.fn = fn;
	data.userdata = userdata;
	prev = -1;
	while(data.current < data.target)
	{
		if(sql_perform(sql, sql_migrate_txn_, &data, 10))
		{
			return -1;
		}
		if(data.current <= prev)
		{			
			/* Nothing changed, abort */
			return -1;
		}
		prev = data.current;
	}
	return 0;
}

/* A transaction callback returns 0 for commit, -1 for rollback and retry, 1 for rollback successfully */
static int
sql_migrate_txn_(SQL *sql, void *userdata)
{
	struct migrate_data *data;
	int v;
	
	data = (struct migrate_data *) userdata;

	/* Confirm current version */
	v = sql->api->schema_get_version(sql, data->identifier);
	if(sql->api->deadlocked(sql))
	{
		/* Re-try the transaction */
		return -1;
	}
	if(v == -1)
	{
		if(strcmp(sql->api->sqlstate(sql), "42S02"))
		{
			/* An error occurred */
			return 1;
		}
		/* Table doesn't exist */
		v = sql->api->schema_create_table(sql);
		if(sql->api->deadlocked(sql))
		{			
			return -1;
		}
		if(v)
		{
			return 1;
		}
		v = sql->api->schema_set_version(sql, data->identifier, 0);
		if(sql->api->deadlocked(sql))
		{
			return -1;
		}
		if(v)
		{
			return 1;
		}
		data->current = v;
		return 0;
	}
	data->current = v;
	if(data->current == data->target)
	{
		return 1;
	}
	v = data->fn(sql, data->identifier, data->current + 1, data->userdata);
	if(sql->api->deadlocked(sql))
	{
		return -1;
	}
	if(v)
	{		
		return 1;
	}
	v = sql->api->schema_set_version(sql, data->identifier, data->current + 1);
	if(sql->api->deadlocked(sql))
	{
		return -1;
	}
	if(v != data->current + 1)
	{
		return 1;
	}
	data->current = v;
	return 0;
}
