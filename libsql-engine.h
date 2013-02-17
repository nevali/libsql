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
# include <uuid/uuid.h>

extern const uuid_t sql_uuid_unknown;
extern const uuid_t sql_uuid_engine;
extern const uuid_t sql_uuid_sql;
extern const uuid_t sql_uuid_statement;
extern const uuid_t sql_uuid_field;

typedef struct sql_engine_struct SQL_ENGINE;

typedef struct sql_engine_api_struct SQL_ENGINE_API;
typedef struct sql_api_struct SQL_API;
typedef struct sql_statement_api_struct SQL_STATEMENT_API;
typedef struct sql_field_api_struct SQL_FIELD_API;

/* API implemented by SQL engine plug-ins */
struct sql_engine_api_struct
{
	int (*queryinterface)(SQL_ENGINE *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
	unsigned long (*addref)(SQL_ENGINE *me);
	unsigned long (*release)(SQL_ENGINE *me);
	SQL *(*create)(SQL_ENGINE *me);
};

/* API provided by an individual connection */
struct sql_api_struct
{
	int (*queryinterface)(SQL *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
	unsigned long (*addref)(SQL *me);
	unsigned long (*release)(SQL *me);
	int (*lock)(SQL *me);
	int (*unlock)(SQL *me);
	int (*trylock)(SQL *me);
	void *reserved_escape;
	const char *(*sqlstate)(SQL *me);
	const char *(*error)(SQL *me);
	int (*connect)(SQL *restrict me, URI *restrict uri);
	int (*execute)(SQL *restrict me, const char *restrict statement, void *restrict *restrict data);
	SQL_STATEMENT *(*statement)(SQL *restrict me, const char *restrict statement);
};

/* API provided on statements */
struct sql_statement_api_struct
{
	int (*queryinterface)(SQL_STATEMENT *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
	unsigned long (*addref)(SQL_STATEMENT *me);
	unsigned long (*release)(SQL_STATEMENT *me);
	const char *(*statement)(SQL_STATEMENT *me);
	int (*set_results)(SQL_STATEMENT *restrict me, void *restrict data);
	unsigned int (*columns)(SQL_STATEMENT *me);
	unsigned long long (*rows)(SQL_STATEMENT *me);
	unsigned long long (*affected)(SQL_STATEMENT *me);
	SQL_FIELD *(*field)(SQL_STATEMENT *restrict me, unsigned int col);
	int (*null)(SQL_STATEMENT *me, unsigned int col);
	size_t (*value)(SQL_STATEMENT *restrict me, unsigned int col, char *restrict buf, size_t buflen);
	const unsigned char *(*valueptr)(SQL_STATEMENT *me, unsigned int col);
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
	int (*queryinterface)(SQL_FIELD *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
	unsigned long (*addref)(SQL_FIELD *me);
	unsigned long (*release)(SQL_FIELD *me);
	const char *(*name)(SQL_FIELD *me);
	size_t (*width)(SQL_FIELD *me);
};

#define SQL_ENGINE_COMMON_MEMBERS \
	SQL_ENGINE_API *api; \
	unsigned long refcount;

#define SQL_COMMON_MEMBERS \
	SQL_API *api; \
	unsigned long refcount; \
	pthread_mutex_t lock;

#define SQL_STATEMENT_COMMON_MEMBERS \
	SQL_STATEMENT_API *api; \
	unsigned long refcount;

#define SQL_FIELD_COMMON_MEMBERS \
	SQL_FIELD_API *api; \
	unsigned long refcount;

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

int sql_engine_def_queryinterface_(SQL_ENGINE *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
unsigned long sql_engine_def_addref_(SQL_ENGINE *me);
unsigned long sql_engine_def_release_(SQL_ENGINE *me);

int sql_def_queryinterface_(SQL *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
unsigned long sql_def_addref_(SQL *me);
int sql_def_lock_(SQL *me);
int sql_def_unlock_(SQL *me);
int sql_def_trylock_(SQL *me);

int sql_statement_def_queryinterface_(SQL_STATEMENT *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
unsigned long sql_statement_def_addref_(SQL_STATEMENT *me);

int sql_field_def_queryinterface_(SQL_FIELD *restrict me, uuid_t *restrict uuid, void *restrict *restrict out);
unsigned long sql_field_def_addref_(SQL_FIELD *me);

#endif /*!LIBSQL_ENGINE_H_*/
