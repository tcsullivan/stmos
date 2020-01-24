/**
 * @file crt0.c
 * Userland executable startup code
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

extern int main(int, char **);

static void stdio_init(void);

/**
 * Entry point for userland programs.
 * Sets up stdio, then enters the program's main() before exiting with main()'s
 * return code.
 */
void _start(void)
{
	stdio_init();
	exit(main(0, 0));
}

/**
 * Initializes file handles for stdout, stdin, and stderr.
 */
void stdio_init(void)
{
	stderr = calloc(1, sizeof(FILE));
	stderr->handle = 2;
	stderr->buffer = malloc(BUFSIZ);
	stderr->bufsize = BUFSIZ;
	stderr->ungetbuf = malloc(_PDCLIB_UNGETCBUFSIZE);
	stderr->status = _IOLBF | _PDCLIB_FWRITE | _PDCLIB_STATIC;
	stderr->next = NULL;

	stdout = calloc(1, sizeof(FILE));
	stdout->handle = 1;
	stdout->buffer = malloc(BUFSIZ);
	stdout->bufsize = BUFSIZ;
	stdout->ungetbuf = malloc(_PDCLIB_UNGETCBUFSIZE);
	stdout->status = _IOLBF | _PDCLIB_FWRITE | _PDCLIB_STATIC;
	stdout->next = stderr;

	stdin = calloc(1, sizeof(FILE));
	stdin->handle = 0;
	stdin->buffer = malloc(BUFSIZ);
	stdin->bufsize = BUFSIZ;
	stdin->ungetbuf = malloc(_PDCLIB_UNGETCBUFSIZE);
	stdin->status = _IOLBF | _PDCLIB_FREAD | _PDCLIB_STATIC;
	stdin->next = stdout;
}

