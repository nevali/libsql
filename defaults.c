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

#define SQL_STRUCT_DEFINED             1

#include "p_libsql.h"

/* {00000000-0000-0000-c000-000000000046} */
const uuid_t sql_uuid_unknown = {
	0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x46
};

/* {7b58ac43-8587-413d-8f67-a289c8e122ed} */
const uuid_t sql_uuid_engine = {
	0x7b, 0x58, 0xac, 0x43, 
	0x85, 0x87, 0x41, 0x3d, 0x8f, 0x67, 
	0xa2, 0x89, 0xc8, 0xe1, 0x22, 0xed
};

/* {f42ef4d1-5212-4789-8f6c-97f726dde8c1} */
const uuid_t sql_uuid_sql = {
	0xf4, 0x2e, 0xf4, 0xd1, 
	0x52, 0x12, 0x47, 0x89, 0x8f, 0x6c, 
	0x97, 0xf7, 0x26, 0xdd, 0xe8, 0xc1
};

/* {062a4312-cab7-43bf-abd4-6206b17b864f} */
const uuid_t sql_uuid_statement = {
	0x06, 0x2a, 0x43, 0x12, 
	0xca, 0xb7, 0x43, 0xbf, 0xab, 0xd4, 
	0x62, 0x06, 0xb1, 0x7b, 0x86, 0x4f
};

/* {31c9fd20-0661-4b2f-a549-e638b92086cb} */
const uuid_t sql_uuid_field = {
	0x31, 0xc9, 0xfd, 0x20, 
	0x06, 0x61, 0x4b, 0x2f, 0xa5, 0x49, 
	0xe6, 0x38, 0xb9, 0x20, 0x86, 0xcb
};

struct sql_engine_struct { SQL_ENGINE_COMMON_MEMBERS };
struct sql_struct { SQL_COMMON_MEMBERS };
struct sql_statement_struct { SQL_STATEMENT_COMMON_MEMBERS };
struct sql_field_struct { SQL_FIELD_COMMON_MEMBERS };

int
sql_engine_def_queryinterface_(SQL_ENGINE *restrict me, uuid_t *restrict uuid, void *restrict *restrict out)
{
	if(!uuid_compare(*uuid, sql_uuid_unknown) || !uuid_compare(*uuid, sql_uuid_engine))
	{
		*out = me;
		return 0;
	}
	*out = NULL;
	errno = ENOENT;
	return -1;
}

unsigned long
sql_engine_def_addref_(SQL_ENGINE *me)
{
	(void) me;
	
	/* Do nothing */
	return 1;
}

unsigned long
sql_engine_def_release_(SQL_ENGINE *me)
{
	(void) me;
	
	/* Do nothing */
	return 1;
}

int
sql_def_queryinterface_(SQL *restrict me, uuid_t *restrict uuid, void *restrict *restrict out)
{
	if(!uuid_compare(*uuid, sql_uuid_unknown) || !uuid_compare(*uuid, sql_uuid_sql))
	{
		*out = me;
		return 0;
	}
	*out = NULL;
	errno = ENOENT;
	return -1;
}

unsigned long
sql_def_addref_(SQL *me)
{
	me->refcount++;
	return me->refcount;
}

int
sql_def_lock_(SQL *me)
{
	pthread_mutex_lock(&(me->lock));
	return 0;
}

int
sql_def_unlock_(SQL *me)
{
	pthread_mutex_unlock(&(me->lock));
	return 0;
}

int
sql_def_trylock_(SQL *me)
{
	return pthread_mutex_trylock(&(me->lock));
}


int
sql_statement_def_queryinterface_(SQL_STATEMENT *restrict me, uuid_t *restrict uuid, void *restrict *restrict out)
{
	if(!uuid_compare(*uuid, sql_uuid_unknown) || !uuid_compare(*uuid, sql_uuid_statement))
	{
		*out = me;
		return 0;
	}
	*out = NULL;
	errno = ENOENT;
	return -1;
}

unsigned long
sql_statement_def_addref_(SQL_STATEMENT *me)
{
	me->refcount++;
	return me->refcount;
}

int
sql_field_queryinterface_(SQL_FIELD *restrict me, uuid_t *restrict uuid, void *restrict *restrict out)
{
	if(!uuid_compare(*uuid, sql_uuid_unknown) || !uuid_compare(*uuid, sql_uuid_field))
	{
		*out = me;
		return 0;
	}
	*out = NULL;
	errno = ENOENT;
	return -1;
}

unsigned long
sql_field_addref_(SQL_FIELD *me)
{
	me->refcount++;
	return me->refcount;
}
