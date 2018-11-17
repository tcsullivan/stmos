#include "vfs.h"

#include <kernel/task.h>
#include <fs/stdio.h>

// +1 vol for stdio, +3 fd's for stdout, in, err
#define VFS_MAX_VOLS  (1 + 8)
#define VFS_MAX_FILES (3 + 10)

static vfs_volume vfs_volumes[VFS_MAX_VOLS];
static vfs_file vfs_files[VFS_MAX_FILES];

void vfs_svc(uint32_t *args)
{
	switch (args[0]) {
	case 0:
		*((int *)args[3]) = vfs_mount((vfs_volume_funcs *)args[1],
			args[2]);
		break;
	case 1:
		*((int *)args[3]) = vfs_open((const char *)args[1], args[2]);
		break;
	case 2:
		*((int *)args[2]) = vfs_close(args[1]);
		break;
	case 3:
		*((int *)args[4]) = vfs_read(args[1], args[2], (uint8_t *)args[3]);
		break;
	default:
		break;
	}
}

void vfs_init(void)
{
	for (int i = 0; i < VFS_MAX_VOLS; i++)
		vfs_volumes[i].flags = 0;
	for (int i = 0; i < VFS_MAX_FILES; i++)
		vfs_files[i].flags = 0;

	vfs_mount(&stdio_funcs, 0);
	// order is crucial
	vfs_open("in", VFS_FILE_READ);
	vfs_open("out", VFS_FILE_WRITE);
	vfs_open("err", VFS_FILE_WRITE);
}

int vfs_mount(vfs_volume_funcs *funcs, uint32_t flags)
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
	if (path[0] == '\0' || path[1] == '\0' || path[1] != ':' ||
		path[2] == '\0' || path[2] != '/')
		return -1;

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

	vfs_files[file].flags = VFS_FILE_OPEN | flags;
	vfs_files[file].vol = drive;
	vfs_files[file].pid = task_getpid();
	vfs_files[file].fsinfo =
		vfs_volumes[drive].funcs->open(path + 3);

	return file;
}

int vfs_close(int fd)
{
	if (fd < 0 || fd > VFS_MAX_FILES)
		return -1;
	if (vfs_volumes[vfs_files[fd].vol].funcs->close == 0)
		return -1;
	if (vfs_files[fd].pid != task_getpid())
		return -1;
	if (!(vfs_files[fd].flags & VFS_FILE_OPEN))
		return 0;

	// TODO care
	/*int ret =*/ vfs_volumes[vfs_files[fd].vol].funcs->close(
		vfs_files[fd].fsinfo);

	vfs_files[fd].flags = 0;
	vfs_files[fd].pid = 0;
	return 0;
}

uint32_t vfs_read(int fd, uint32_t count, uint8_t *buffer)
{
	if (fd < 0 || fd > VFS_MAX_FILES || count == 0 || buffer == 0)
		return 0;
	if (vfs_volumes[vfs_files[fd].vol].funcs->read == 0)
		return -1;
	if (vfs_files[fd].pid != task_getpid())
		return 0;
	if ((!(vfs_files[fd].flags & VFS_FILE_READ)) || (vfs_files[fd].flags &
		VFS_EOF))
		return 0;

	uint32_t ret = vfs_volumes[vfs_files[fd].vol].funcs->read(vfs_files[fd].fsinfo,
		count, buffer);

	if (ret < count)
		vfs_files[fd].flags |= VFS_EOF;

	return ret;
}

