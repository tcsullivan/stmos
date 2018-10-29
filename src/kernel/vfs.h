#ifndef VFS_H_
#define VFS_H_

#include <stdint.h>

struct vfs_node;
struct dirent;

typedef struct {
	uint32_t (*read)(struct vfs_node *, uint32_t, uint32_t, uint8_t *);
	uint32_t (*write)(struct vfs_node *, uint32_t, uint32_t, const uint8_t *);
	void (*open)(struct vfs_node *);
	void (*close)(struct vfs_node *);
} vfs_driver;

typedef struct vfs_node {
	char *name;
	uint32_t flags;

	struct vfs_node *parent;
	union {
		struct vfs_node* children;
		vfs_driver *access;
	} data;
} vfs_node;

struct dirent {
	char *name;
	vfs_node *node;
};

#endif // VFS_H_
