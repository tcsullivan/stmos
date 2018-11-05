#ifndef VFS_H_
#define VFS_H_

#include <stdint.h>

typedef struct {
	char name[32];
} vfs_dirent;

typedef struct {
	void *(*open)(const char *file);
	void (*close)(void *info);
	uint32_t (*read)(void *info, uint32_t count, uint8_t *buffer);
	uint32_t (*write)(void *info, uint32_t count, const uint8_t *buffer);
	vfs_dirent *(*readdir)(const char *path);
} vfs_volume_funcs;

#define VFS_MOUNTED  (1 << 0)
#define VFS_READONLY (1 << 1)

typedef struct {
	uint32_t flags;
	vfs_volume_funcs *funcs;
} vfs_volume;

#define VFS_FILE_OPEN  (1 << 0)
#define VFS_FILE_READ  (1 << 1)
#define VFS_FILE_WRITE (1 << 2)
#define VFS_FILE_MODF  (1 << 3)
#define VFS_TEMPORARY  (1 << 4)
#define VFS_EOF        (1 << 5)

typedef struct {
	uint32_t flags;
	uint32_t vol;
	uint32_t pid;
	void *fsinfo;
} vfs_file;

void vfs_init(void);

int vfs_mount(vfs_volume_funcs *funcs, uint32_t flags);
int vfs_open(const char *path, uint32_t flags);
uint32_t vfs_read(int fd, uint32_t count, uint8_t *buffer);

#endif // VFS_H_
