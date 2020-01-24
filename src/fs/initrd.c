/**
 * @file initrd.c
 * Filesystem module for handling the initrd
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

#include <stdint.h>

#include <kernel/heap.h>
#include <kernel/vfs.h>

typedef struct {
	char *address;
	uint32_t size;
} initrd_info;

// Defined by linker script
extern uint8_t _binary_initrd_img_start[];
extern uint8_t _binary_initrd_img_size[];

static const uint8_t *initrd_start = (uint8_t *)_binary_initrd_img_start;
static const uint32_t initrd_size = (uint32_t)_binary_initrd_img_size;

void *initrd_open(const char *file);
uint32_t initrd_read(vfs_file_info *info, uint32_t count, uint8_t *buffer);
int initrd_close(vfs_file_info *info);
int initrd_seek(vfs_file_info *info, uint32_t offset, int whence);

/**
 * Gets the start of the offset-th file in the initrd.
 * @return Pointer to the file, NULL otherwise
 */
char *initrd_getfile(uint32_t offset);

static const vfs_volume_funcs initrd_funcs = {
	initrd_open,
	initrd_close,
	initrd_read,
	0, // write
	0, // readdir
	initrd_seek
};

/**
 * Custom strncmp() implementation used by this driver.
 */
int initrd_strncmp(const char *a, const char *b, unsigned int n)
{
	for (unsigned int i = 0; i < n; i++) {
		if (a[i] != b[i])
			return 1;
	}

	return 0;
}

void initrd_init(void)
{
	vfs_mount(&initrd_funcs, VFS_READONLY);
}

void *initrd_open(const char *file)
{
    // Iterate through all files in the initrd
	char *ptr;
	for (uint32_t i = 0; ptr = initrd_getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!initrd_strncmp(file, ptr + 4, len)) {
            // If we found our file, create an info structure and return it
			initrd_info *file = (initrd_info *)malloc(
				sizeof(initrd_info));
			file->address = ptr + len + 8;
			file->size = *(uint32_t *)(ptr + len + 4);
			return file;
		}
	}

	return 0;
}

int initrd_close(vfs_file_info *info)
{
	free(info->fsinfo);
	return 0;
}

uint32_t initrd_read(vfs_file_info *info, uint32_t count, uint8_t *buffer)
{
    // Confirm this is a valid file
	initrd_info *iinfo = (initrd_info *)info->fsinfo;
	if (iinfo == 0 || iinfo->address == 0)
		return 0;
	
    // Attempt to read 'count' bytes, breaking if we reach the end of the file
	uint32_t i;
	for (i = 0; i < count; i++) {
		if (info->pos >= iinfo->size)
			break;

		buffer[info->pos] = iinfo->address[info->pos];
		info->pos++;
	}

	return i;
}

int initrd_seek(vfs_file_info *info, uint32_t offset, int whence)
{
	initrd_info *iinfo = (initrd_info *)info->fsinfo;
	if (iinfo == 0 || iinfo->address == 0)
		return 0;

	if (whence == SEEK_SET)
		info->pos = offset;
	else if (whence == SEEK_CUR)
		info->pos += offset;
	else if (whence == SEEK_END)
		info->pos = iinfo->size + offset;
	else
		return -1;

	return 0;
}

char *initrd_getfile(uint32_t offset)
{
	char *ptr = (char *)initrd_start;
	for (uint32_t i = 0; i < offset; i++) {
        // Move to the next file
		uint32_t len = *((uint32_t *)ptr);
		uint32_t datalen = *((uint32_t *)(ptr + 4 + len));
		ptr += len + datalen + 8;

		if (ptr >= (char *)(initrd_start + initrd_size))
			return 0;
	}

	return ptr;
}

