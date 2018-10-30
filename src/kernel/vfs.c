#include "vfs.h"
#include "task.h"

typedef unsigned int size_t;

//static vfs_node vfs_root;

int vfs_open(const char *path, int flags, int mode);
size_t vfs_read(int fd, void *buf, size_t count);
size_t vfs_write(int fd, const void *buf, size_t count);
int vfs_close(int fd);

void vfs_svc(uint32_t *args)
{
	switch (args[0]) {
	case 0:
		*((int *)args[4]) = vfs_open(args[1], args[2], args[3]);
		break;
	case 1:
		*((int *)args[4]) = vfs_read(args[1], args[2], args[3]);
		break;
	case 2:
		*((int *)args[4]) = vfs_write(args[1], args[2], args[3]);
		break;
	case 3:
		*((int *)args[2]) = vfs_close(args[1]);
		break;
	}
}

