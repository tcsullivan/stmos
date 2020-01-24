/**
 * @file vfs.c
 * An implementation of filesystem abstraction
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

#include "vfs.h"

#include <kernel/task.h>
#include <fs/stdio.h>

// +1 vol for stdio, +3 fd's for stdout, in, err
#define VFS_MAX_VOLS  (1 + 8)
#define VFS_MAX_FILES (3 + 10)

static vfs_volume vfs_volumes[VFS_MAX_VOLS];
static vfs_file vfs_files[VFS_MAX_FILES];

#define VFS_PID_CHECK(fpid) { \
	uint32_t pid = task_getpid(); \
	if (pid != fpid && fpid != 0) \
		return 0; }

void vfs_svc(uint32_t n, uint32_t *ret, uint32_t *args)
{
	switch (n) {
	case 0:
		*((int *)ret) = vfs_mount((vfs_volume_funcs *)args[0],
			args[1]);
		break;
	case 1:
		*((int *)ret) = vfs_open((const char *)args[0], args[1]);
		break;
	case 2:
		*((int *)ret) = vfs_close(args[0]);
		break;
	case 3:
		*((int *)ret) = vfs_read(args[0], args[1], (uint8_t *)args[2]);
		break;
	case 4:
		*((int *)ret) = vfs_write(args[0], args[1], (const uint8_t *)args[2]);
		break;
	default:
		break;
	}
}

void vfs_init(void)
{
    // Mark all volumes and files as empty
	for (int i = 0; i < VFS_MAX_VOLS; i++)
		vfs_volumes[i].flags = 0;
	for (int i = 0; i < VFS_MAX_FILES; i++)
		vfs_files[i].flags = 0;

	vfs_mount(&stdio_funcs, 0);
    // Order of opening here may be important TODO confirm
	vfs_open("   in", VFS_FILE_READ);
	vfs_open("   out", VFS_FILE_WRITE);
	vfs_open("   err", VFS_FILE_WRITE);
}

int vfs_mount(const vfs_volume_funcs *funcs, uint32_t flags)
{
	for (int i = 0; i < VFS_MAX_VOLS; i++) {
		if (!(vfs_volumes[i].flags && VFS_MOUNTED)) {
			vfs_volumes[i].flags = VFS_MOUNTED | flags;
			vfs_volumes[i].funcs = funcs;
			return i;
		}
	}

	return -1;
}

int vfs_get_drive(const char *path)
{
	// Validate parameters
	if (path[0] == '\0')
		return -1;

	// Default to 'A' if no drive specified (A gives stdio)
	if (path[1] == '\0' || path[1] != ':' ||
		path[2] == '\0' || path[2] != '/')
		return 0;

	// Find chosen drive
	int drive = -1;
	for (int i = 0; i < VFS_MAX_VOLS; i++) {
		if (path[0] == ('a' + i) || path[0] == ('A' + i)) {
			drive = i;
			break;
		}
	}

	if (drive == -1 || !(vfs_volumes[drive].flags && VFS_MOUNTED))
		return -1;

	return drive;
}

int vfs_open(const char *path, uint32_t flags)
{
	int drive = vfs_get_drive(path);
	if (drive == -1)
		return -1;
	if (vfs_volumes[drive].funcs->open == 0)
		return -1;

	// Find available file handle
	int file = -1;
	for (int i = 0; i < VFS_MAX_FILES; i++) {
		if (!(vfs_files[i].flags & VFS_FILE_OPEN)) {
			file = i;
			break;
		}
	}

	if (file == -1)
		return -1;

	void *fsinfo = vfs_volumes[drive].funcs->open(path + 3);
	if (fsinfo == 0)
		return -1;

	vfs_files[file].flags = VFS_FILE_OPEN | flags;
	vfs_files[file].vol = drive;
	vfs_files[file].pid = task_getpid();
	vfs_files[file].info.pos = 0;
	vfs_files[file].info.fsinfo = fsinfo;

	return file;
}

int vfs_close(int fd)
{
	if (fd < 0 || fd > VFS_MAX_FILES)
		return -1;
	if (vfs_volumes[vfs_files[fd].vol].funcs->close == 0)
		return -1;
	VFS_PID_CHECK(vfs_files[fd].pid);
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return 0;

	// TODO care
	/*int ret =*/ vfs_volumes[vfs_files[fd].vol].funcs->close(
		&vfs_files[fd].info);

	vfs_files[fd].flags = 0;
	vfs_files[fd].pid = 0;
	return 0;
}

uint32_t vfs_read(int fd, uint32_t count, uint8_t *buffer)
{
	if (fd < 0 || fd > VFS_MAX_FILES || count == 0 || buffer == 0)
		return 0;
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return -1;
	if (vfs_volumes[vfs_files[fd].vol].funcs->read == 0)
		return -1;
	VFS_PID_CHECK(vfs_files[fd].pid);
	if ((!(vfs_files[fd].flags & VFS_FILE_READ)) || (vfs_files[fd].flags &
		VFS_EOF))
		return 0;

	uint32_t ret = vfs_volumes[vfs_files[fd].vol].funcs->read(
		&vfs_files[fd].info, count, buffer);

	if (ret < count)
		vfs_files[fd].flags |= VFS_EOF;

	return ret;
}

uint32_t vfs_write(int fd, uint32_t count, const uint8_t *buffer)
{
	if (fd < 0 || fd > VFS_MAX_FILES || count == 0 || buffer == 0)
		return 0;
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return -1;
	if (vfs_volumes[vfs_files[fd].vol].funcs->write == 0)
		return -1;
	VFS_PID_CHECK(vfs_files[fd].pid);
	// TODO append?
	if ((!(vfs_files[fd].flags & VFS_FILE_WRITE)) || (vfs_files[fd].flags &
		VFS_EOF))
		return 0;

	uint32_t ret = vfs_volumes[vfs_files[fd].vol].funcs->write(
		&vfs_files[fd].info, count, buffer);

	if (ret < count)
		vfs_files[fd].flags |= VFS_EOF;

	return ret;
}

int vfs_seek(int fd, int32_t offset, int whence)
{
	if (fd < 0 || fd > VFS_MAX_FILES)
		return -1;
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return -1;
	if (vfs_volumes[vfs_files[fd].vol].funcs->seek == 0)
		return -1;
	VFS_PID_CHECK(vfs_files[fd].pid);

	return vfs_volumes[vfs_files[fd].vol].funcs->seek(&vfs_files[fd].info,
		offset, whence);
}

int32_t vfs_tell(int fd)
{
	if (fd < 0 || fd > VFS_MAX_FILES)
		return -1;
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return -1;
	VFS_PID_CHECK(vfs_files[fd].pid);

	return (int32_t)vfs_files[fd].info.pos;
}

