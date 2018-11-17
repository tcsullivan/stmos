#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>

//
// Task-related calls

void _exit(int code);

int fork(void);
int getpid(void);
void *sbrk(unsigned int bytes);

//
// Clock-related calls

void delay(unsigned int ms);
unsigned int ticks(void);

//
// File-related calls

// Indicates mounted volume
#define VFS_MOUNTED  (1 << 0)
// Set if filesystem is read-only
#define VFS_READONLY (1 << 1)

// Indicates an opened file
#define VFS_FILE_OPEN  (1 << 0)
// Indicates read permission on file
#define VFS_FILE_READ  (1 << 1)
// Indicates write permission on file
#define VFS_FILE_WRITE (1 << 2)
// Set if EOF has been reached
#define VFS_EOF        (1 << 3)

#define EOF (-1)

struct dirent {
	char name[32];
};

struct vfs_volume_funcs_t;
typedef struct vfs_volume_funcs_t vfs_volume_funcs;

int mount(vfs_volume_funcs *funcs, uint32_t flags);
int open(const char *path, uint32_t flags);
int close(int fd);
int read(int fd, uint32_t count, uint8_t *buffer);
int write(int fd, uint32_t count, const uint8_t *buffer);

#endif // SYSCALLS_H_
