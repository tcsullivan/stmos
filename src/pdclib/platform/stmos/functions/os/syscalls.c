#include "syscalls.h"

#define NOOPTIMIZE __attribute__((optimize(0)))

//
// Task-related calls

NOOPTIMIZE
void _exit(int code)
{
	uint32_t args[1] = { code };
	__asm volatile("\
		mov r0, 0; \
		mov r1, 0; \
		mov r2, %0; \
		svc 0; \
	" :: "r" (args));
}

NOOPTIMIZE
int fork(void)
{
	volatile uint32_t ret = 0;
	__asm volatile("\
		mov r0, 1; \
		mov r1, %0; \
		mov r2, 0; \
		svc 0; \
	" :: "r" (&ret));
	return ret;
}

NOOPTIMIZE
int getpid(void)
{
	volatile uint32_t ret = 0;
	__asm volatile("\
		mov r0, 2; \
		mov r1, %0; \
		mov r2, 0; \
		svc 0; \
	" :: "r" (&ret));
	return ret;
}

NOOPTIMIZE
void *sbrk(unsigned int bytes)
{
	volatile uint32_t ret = 0;
	uint32_t args[1] = { bytes };
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		svc 0; \
	" :: "r" (&ret), "r" (args));
	return (void *)ret;
}

NOOPTIMIZE
int execve(const char *file, char * const argv[], char * const envp[])
{
	volatile uint32_t ret = 0;
	uint32_t args[3] = { (uint32_t)file, (uint32_t)argv, (uint32_t)envp };
	__asm("\
		mov r0, 5; \
		mov r1, %0; \
		mov r2, %1; \
		svc 0; \
	" :: "r" (&ret), "r" (args));

	if (ret == (uint32_t)-1)
		return ret;

	((void (*)(void))ret)();
}

//
// Clock-related calls

NOOPTIMIZE
void delay(unsigned int ms)
{
	uint32_t args[1] = { ms };
	__asm("\
		mov r0, 0; \
		mov r1, 0; \
		mov r2, %0; \
		svc 2; \
	" :: "r" (args));
}

NOOPTIMIZE
unsigned int ticks(void)
{
	volatile unsigned int ret = 0;
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		mov r2, 0; \
		svc 2; \
	" :: "r" (&ret));
	return ret;
}

//
// File-related calls

NOOPTIMIZE
int mount(vfs_volume_funcs *funcs, uint32_t flags)
{
	volatile int ret = 0;
	uint32_t args[2] = { (uint32_t)funcs, flags };
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

NOOPTIMIZE
int open(const char *path, uint32_t flags)
{
	volatile int ret = 0;
	uint32_t args[2] = { (uint32_t)path, flags };
	__asm("\
		mov r0, 1; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

NOOPTIMIZE
int close(int fd)
{
	volatile int ret = 0;
	uint32_t args[1] = { fd };
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

NOOPTIMIZE
int read(int fd, uint32_t count, uint8_t *buffer)
{
	volatile int ret = 0;
	uint32_t args[3] = { fd, count, (uint32_t)buffer };
	__asm("\
		mov r0, 3; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

NOOPTIMIZE
int write(int fd, uint32_t count, const uint8_t *buffer)
{
	volatile int ret = 0;
	uint32_t args[3] = { fd, count, (uint32_t)buffer };
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		svc 3; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

