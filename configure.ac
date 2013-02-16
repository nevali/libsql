## Copyright 2012-2013 Mo McRoberts.
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.

AC_INIT([libsql],[master])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

AC_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CXX
AC_HEADER_STDC
AC_PROG_LIBTOOL

AC_CHECK_HEADERS([limits.h stddef.h])

LIBSQL_CC_WARN
LIBSQL_PREFIX
LIBSQL_CHECK_LIBURI
LIBSQL_CHECK_MYSQL
LIBSQL_CHECK_LIBEDIT
LIBSQL_CHECK_UUID

AC_SUBST([ENGINE_SUBDIRS])

AC_CONFIG_FILES([Makefile
mysql/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([

enabling engines:

           mysql ... $engine_mysql

])
