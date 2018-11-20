#ifndef VFS_H_
#define VFS_H_

#include <stdint.h>

typedef struct {
	char name[32];
} vfs_dirent;

typedef struct vfs_volume_funcs_t {
	void *(*open)(const char *file);
	int (*close)(void *info);
	uint32_t (*read)(void *info, uint32_t count, uint8_t *buffer);
	uint32_t (*write)(void *info, uint32_t count, const uint8_t *buffer);
	vfs_dirent *(*readdir)(const char *path);
} vfs_volume_funcs;

// Indicates mounted volume
#define VFS_MOUNTED  (1 << 0)
// Set if filesystem is read-only
#define VFS_READONLY (1 << 1)

typedef struct {
	uint32_t flags;
	const vfs_volume_funcs *funcs;
} vfs_volume;

// Indicates an opened file
#define VFS_FILE_OPEN  (1 << 0)
// Indicates read permission on file
#define VFS_FILE_READ  (1 << 1)
// Indicates write permission on file
#define VFS_FILE_WRITE (1 << 2)
// Set if EOF has been reached
#define VFS_EOF        (1 << 3)

typedef struct {
	uint32_t flags; /**< File attribute flags */
	uint32_t vol;   /**< Index of volume file is stored on */
	uint32_t pid;   /**< PID of process handling this file */
	void *fsinfo;   /**< Filesystem-specific data, handled by fs driver */
} vfs_file;

void vfs_init(void);

int vfs_mount(const vfs_volume_funcs *funcs, uint32_t flags);
int vfs_open(const char *path, uint32_t flags);
int vfs_close(int fd);
uint32_t vfs_read(int fd, uint32_t count, uint8_t *buffer);
uint32_t vfs_write(int fd, uint32_t count, const uint8_t *buffer);

#endif // VFS_H_
