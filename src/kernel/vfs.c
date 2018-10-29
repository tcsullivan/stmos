#include "vfs.h"
#include "task.h"

//static vfs_node vfs_root;

int vfs_open(const char *path, int mode)
{
	(void)path;
	(void)mode;
//	vfs_node *cd = task_getcwd();
//	if (cd == 0)
//		cd = &vfs_root;

	return 0;
}
