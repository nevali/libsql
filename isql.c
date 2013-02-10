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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "histedit.h"
#include "libsql.h"

static char *
prompt(EditLine *el)
{
	(void) el;

	return "SQL> ";
}

static int
show_results(SQL_STATEMENT *rs, unsigned int cols)
{
	size_t d, max, len;
	size_t *widths;
	unsigned int c;
	SQL_FIELD **fields;
	const char *n;
	unsigned long long rows;
	char *fbuf;
	
	fbuf = NULL;
	max = 0;
	fields = (SQL_FIELD **) calloc(cols, sizeof(SQL_FIELD *));
	widths = (size_t *) calloc(cols, sizeof(size_t));
	putchar('+');	
	for(c = 0; c < cols; c++)
	{
		fields[c] = sql_stmt_field(rs, c);
		widths[c] = sql_field_width(fields[c]);
		n = sql_field_name(fields[c]);
		if(n && strlen(n) > widths[c])
		{
			widths[c] = strlen(n);
		}
		if(widths[c] > max)
		{
			max = widths[c];
		}
		for(d = 0; d < widths[c] + 2; d++)
		{
			putchar('-');
		}
		putchar('+');
	}
	fbuf = (char *) malloc(max + 1);
	putchar('\n');
	putchar('|');
	for(c = 0; c < cols; c++)
	{
		putchar(' ');
		n = sql_field_name(fields[c]);
		d = widths[c] - strlen(n);
		fputs(n, stdout);
		for(; d; d--)
		{
			putchar(' ');
		}
		putchar(' ');
		putchar('|');
	}
	putchar('\n');
	putchar('+');
	for(c = 0; c < cols; c++)
	{
		for(d = 0; d < widths[c] + 2; d++)
		{
			putchar('-');
		}
		putchar('+');
	}
	putchar('\n');
	while(!sql_stmt_eof(rs))
	{
		putchar('|');
		for(c = 0; c < cols; c++)
		{
			putchar(' ');
			len = sql_stmt_value(rs, c, fbuf, max + 1);
			if(len == (size_t) - 1 || len == 1)
			{
				len = 0;
			}
			else
			{
				len--;
			}
			for(d = 0; d < widths[c]; d++)
			{
				if(d < len)
				{
					putchar(fbuf[d]);
				}
				else
				{
					putchar(' ');
				}
			}
			putchar(' ');
			putchar('|');
		}
		putchar('\n');
		sql_stmt_next(rs);
	}
	putchar('+');
	for(c = 0; c < cols; c++)
	{
		for(d = 0; d < widths[c] + 2; d++)
		{
			putchar('-');
		}
		putchar('+');
	}
	putchar('\n');
	rows = sql_stmt_rows(rs);
	if(rows == 1)
	{
		printf("1 row in set.\n");
	}
	else
	{
		printf("%qu rows in set.\n", rows);
	}
	for(c = 0; c < cols; c++)
	{
		sql_field_destroy(fields[c]);
	}
	free(fbuf);
	free(fields);
	free(widths);
	return 0;
}

int
main(int argc, char **argv)
{
	SQL *conn;
	SQL_STATEMENT *rs;
	EditLine *el;
	History *hist;
	const char *buf;
	char *t;
	int num;
	unsigned int cols;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s URI\n", argv[0]);
		return 1;
	}
	fprintf(stderr, "%s interactive SQL shell (%s)\n\n", PACKAGE, VERSION);
	conn = sql_connect(argv[1]);
	hist = history_init();
	el = el_init(argv[0], stdin, stdout, stderr);
	el_set(el, EL_SIGNAL, 1);
	el_set(el, EL_PROMPT_ESC, prompt);
	el_set(el, EL_HIST, history, hist);
	el_source(el, NULL);
	while((buf = el_gets(el, &num)) != NULL && num != 0)
	{
		t = strchr(buf, '\n');
		if(t)
		{
			*t = 0;
		}
		fprintf(stderr, "[%s]\n", buf);
		rs = sql_query(conn, buf);
		if(!rs)
		{
			fprintf(stderr, "error -- no results\n");
			continue;
		}
		cols = sql_stmt_columns(rs);
		if(!cols)
		{
			printf("%qu rows affected.\n", sql_stmt_affected(rs));
			sql_stmt_destroy(rs);
			continue;
		}
		show_results(rs, cols);
		sql_stmt_destroy(rs);
	}
	return 0;
}
