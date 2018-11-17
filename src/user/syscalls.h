#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>

//
// Task-related calls

inline void _exit(int code)
{
	register uint32_t r1 __asm("r1") = code;
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		svc 0; \
	" :: "r" (r1));
}

inline int fork(void)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)&ret;
	__asm("\
		mov r0, 1; \
		mov r1, %0; \
		svc 0; \
	" :: "r" (r1));
	return ret;
}

inline int getpid(void)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)&ret;
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		svc 0; \
	" :: "r" (r1));
	return ret;
}

inline void *sbrk(unsigned int bytes)
{
	void *ret = 0;
	register uint32_t r1 __asm("r1") = bytes;
	register uint32_t r2 __asm("r2") = (uint32_t)&ret;
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		svc 0; \
	" :: "r" (r1), "r" (r2));
	return ret;
}

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

inline int mount(vfs_volume_funcs *funcs, uint32_t flags)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)funcs;
	register uint32_t r2 __asm("r2") = flags;
	register uint32_t r3 __asm("r3") = (uint32_t)&ret;
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		svc 3; \
	" :: "r" (r1), "r" (r2), "r" (r3));
	return ret;
}

inline int open(const char *path, uint32_t flags)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)path;
	register uint32_t r2 __asm("r2") = flags;
	register uint32_t r3 __asm("r3") = (uint32_t)&ret;
	__asm("\
		mov r0, 1; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		svc 3; \
	" :: "r" (r1), "r" (r2), "r" (r3));
	return ret;
}

inline int close(int fd)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = fd;
	register uint32_t r2 __asm("r2") = (uint32_t)&ret;
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (r1), "r" (r2));
}

inline int read(int fd, uint32_t count, uint8_t *buffer)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = fd;
	register uint32_t r2 __asm("r2") = count;
	register uint32_t r3 __asm("r3") = (uint32_t)buffer;
	register uint32_t r4 __asm("r4") = (uint32_t)&ret;
	__asm("\
		mov r0, 3; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		mov r4, %3; \
		svc 3; \
	" :: "r" (r1), "r" (r2), "r" (r3), "r" (r4));
	return ret;
}

#endif // SYSCALLS_H_
