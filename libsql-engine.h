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

#ifndef LIBSQL_ENGINE_H_
# define LIBSQL_ENGINE_H_               1

# include <libsql.h>

typedef struct sql_engine_struct SQL_ENGINE;

typedef struct sql_engine_api_struct SQL_ENGINE_API;
typedef struct sql_api_struct SQL_API;
typedef struct sql_statement_api_struct SQL_STATEMENT_API;
typedef struct sql_field_api_struct SQL_FIELD_API;

/* API implemented by SQL engine plug-ins */
struct sql_engine_api_struct
{
	void *reserved1;
	void *reserved2;
	void *reserved3;
	SQL *(*create)(SQL_ENGINE *me);
};

/* API provided by an individual connection */
struct sql_api_struct
{
	void *reserved1;
	void *reserved2;
	int (*free)(SQL *me);
	const char *(*sqlstate)(SQL *me);
	const char *(*error)(SQL *me);
	int (*connect)(SQL *restrict me, URI *restrict uri);
	int (*execute)(SQL *restrict me, const char *restrict statement, void *restrict *restrict data);
	SQL_STATEMENT *(*statement)(SQL *restrict me, const char *restrict statement);
};

/* API provided on statements */
struct sql_statement_api_struct
{
	void *reserved1;
	void *reserved2;
	int (*free)(SQL_STATEMENT *me);
	const char *(*statement)(SQL_STATEMENT *me);
	int (*set_results)(SQL_STATEMENT *restrict me, void *restrict data);
	unsigned int (*columns)(SQL_STATEMENT *me);
	unsigned long long (*rows)(SQL_STATEMENT *me);
	unsigned long long (*affected)(SQL_STATEMENT *me);
	SQL_FIELD *(*field)(SQL_STATEMENT *restrict me, unsigned int col);
	int (*null)(SQL_STATEMENT *me, unsigned int col);
	size_t (*value)(SQL_STATEMENT *restrict me, unsigned int col, char *restrict buf, size_t buflen);
	size_t (*valuelen)(SQL_STATEMENT *me, unsigned int col);
	int (*eof)(SQL_STATEMENT *me);
	int (*next)(SQL_STATEMENT *me);
	unsigned long long (*cur)(SQL_STATEMENT *me);
	int (*rewind)(SQL_STATEMENT *me);
	int (*seek)(SQL_STATEMENT *me, unsigned long long ofs);
};

/* API provided on fields */
struct sql_field_api_struct
{
	void *reserved1;
	void *reserved2;
	int (*free)(SQL_FIELD *me);
	const char *(*name)(SQL_FIELD *me);
	size_t (*width)(SQL_FIELD *me);
};

# if !defined(SQL_STRUCT_DEFINED)
struct sql_engine_struct
{
	SQL_ENGINE_API *api;
};

struct sql_struct
{
	SQL_API *api;
};

struct sql_statement_struct
{
	SQL_STATEMENT_API *api;
};

struct sql_field_struct
{
	SQL_FIELD_API *api;
};
# endif /*!SQL_STRUCT_DEFINED*/

#endif /*!LIBSQL_ENGINE_H_*/
