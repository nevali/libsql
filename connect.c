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
		return NULL;
	}
	if(conn->api->connect(conn, uri))
	{
		/* Save error state */
		conn->api->free(conn);
		return NULL;
	}
	return conn;
}
