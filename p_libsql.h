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

#ifndef P_LIBSQL_H_
# define P_LIBSQL_H_                    1

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdarg.h>
# include <pthread.h>
# include <assert.h>
# include <ctype.h>
# ifdef HAVE_LIMITS_H
#  include <limits.h>
# endif
# ifdef HAVE_STDDEF_H
#  include <stddef.h>
# endif

# include <liburi.h>

# include "libsql.h"
# include "libsql-engine.h"

SQL_ENGINE *sql_engine_(URI *uri);

void sql_set_error_(const char *sqlstate, const char *msg);
int sql_vasprintf_query_(char **ptr, const char *format_string, va_list vargs);

#endif
