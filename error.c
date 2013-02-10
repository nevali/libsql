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

static pthread_once_t error_once = PTHREAD_ONCE_INIT;
static pthread_key_t error_key;

static void error_init(void);

struct error_struct
{
	char sqlstate[6];
	char message[512];
};

/* Set the per-thread connectionless SQLSTATE code */
void
sql_set_error_(const char *sqlstate, const char *msg)
{
	struct error_struct *p;
	
	pthread_once(&error_once, error_init);
	p = pthread_getspecific(error_key);
	if(!p)
	{
		p = (struct error_struct *) calloc(1, sizeof(struct error_struct));
		if(!p)
		{
			return;
		}
		pthread_setspecific(error_key, p);
	}
	strncpy(p->sqlstate, sqlstate, sizeof(p->sqlstate) - 1);
	strncpy(p->message, msg, sizeof(p->message) - 1);
}

const char *
sql_sqlstate(SQL *connection)
{
	struct error_struct *p;
	
	if(connection)
	{
		return connection->api->sqlstate(connection);
	}
	pthread_once(&error_once, error_init);
	p = pthread_getspecific(error_key);
	if(!p)
	{
		return "0000";
	}
	return p->sqlstate;
}

const char *
sql_error(SQL *connection)
{
	struct error_struct *p;
	
	if(connection)
	{
		return connection->api->error(connection);
	}
	pthread_once(&error_once, error_init);
	p = pthread_getspecific(error_key);
	if(!p)
	{
		return "No error";
	}
	return p->message;
}

static void
error_init(void)
{
	pthread_key_create(&error_key, NULL);
}