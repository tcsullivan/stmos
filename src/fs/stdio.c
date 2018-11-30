/**
 * @file stdio.c
 * Filesystem module for handling stdio (stdout, stdin, stderr)
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

#include "stdio.h"

#include <kernel/heap.h>
#include <kernel/serial.h>

void *stdio_open(const char *path);
int stdio_close(vfs_file_info *info);
uint32_t stdio_read(vfs_file_info *info, uint32_t count, uint8_t *buffer);
uint32_t stdio_write(vfs_file_info *info, uint32_t count, const uint8_t *buffer);

const vfs_volume_funcs stdio_funcs = {
	stdio_open,
	stdio_close,
	stdio_read,
	stdio_write,
	0, // readdir
	0  // seek
};

void *stdio_open(const char *path)
{
	int *id = malloc(sizeof(uint32_t));

	if (path[0] == 'o' && path[1] == 'u' && path[2] == 't')
		*id = 1;
	else if (path[0] == 'i' && path[1] == 'n')
		*id = 0;
	if (path[0] == 'e' && path[1] == 'r' && path[2] == 'r')
		*id = 2;

	return id;
}

int stdio_close(vfs_file_info *info)
{
	// Nothing to do
	free(info->fsinfo);
	return 0;
}

uint32_t stdio_read(vfs_file_info *info, uint32_t count, uint8_t *buffer)
{
	(void)info;
	(void)count;
	(void)buffer;
	return 0;
}

uint32_t stdio_write(vfs_file_info *info, uint32_t count, const uint8_t *buffer)
{
	(void)info;
	for (uint32_t i = 0; i < count; i++)
		serial_put(buffer[i]);
	return count;
}

