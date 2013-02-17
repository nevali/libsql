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


/* Establish a connection to the database identified by a URI string */
SQL *
sql_connect(const char *uristring)
{
	URI *uri;
	SQL *conn;

	uri = uri_create_str(uristring, NULL);
	if(!uri)
	{
		sql_set_error_("08000", "Failed to parse connection URI");
		return NULL;
	}
	conn = sql_connect_uri(uri);
	uri_destroy(uri);
	return conn;
}

/* Establish a connection to the database identified by a URI */
SQL *
sql_connect_uri(URI *uri)
{
	SQL_ENGINE *engine;
	SQL *conn;
	
	engine = sql_engine_(uri);
	if(!engine)
	{		
		return NULL;
	}
	conn = engine->api->create(engine);
	if(!conn)
	{
		sql_set_error_("53000", "The client engine failed to create a connection object");
		return NULL;
	}
	if(conn->api->connect(conn, uri))
	{
		/* Save error state */
		sql_set_error_(conn->api->sqlstate(conn), conn->api->error(conn));
		conn->api->release(conn);
		return NULL;
	}
	return conn;
}

/* Disconnect from a server */
int
sql_disconnect(SQL *sql)
{
	return sql->api->release(sql);
}

/* Lock a connection, waiting if needed */
int
sql_lock(SQL *sql)
{
	return sql->api->lock(sql);
}

/* Unlock a locked connection */
int
sql_unlock(SQL *sql)
{
	return sql->api->unlock(sql);
}

/* Attempt to lock a connection, fail immediately if already locked */
int
sql_trylock(SQL *sql)
{
	return sql->api->trylock(sql);
}

/* Escape a string */
size_t
sql_escape(SQL *restrict sql, const unsigned char *restrict from, size_t length, char *restrict buf, size_t buflen)
{
	/* Note that:
	 * 1. The return value is always the number of bytes required, including the NULL terminator.
	 * 2. If the return value is greater than buflen, the number of bytes actually written is
	 *    unspecified, except that it will be less than buflen.
	 * 3. It is entirely valid to specify buf and buflen as zero.
	 */
	return sql->api->escape(sql, from, length, buf, buflen);
}
