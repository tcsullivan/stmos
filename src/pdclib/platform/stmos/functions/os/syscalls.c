#include "syscalls.h"

//
// Task-related calls

void _exit(int code)
{
	register uint32_t r1 __asm("r1") = code;
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		svc 0; \
	" :: "r" (r1));
}

int fork(void)
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

int getpid(void)
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

void *sbrk(unsigned int bytes)
{
	uint32_t ret = 0;
	register uint32_t r1 __asm("r1") = bytes;
	register uint32_t r2 __asm("r2") = (uint32_t)&ret;
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		svc 0; \
	" :: "r" (r1), "r" (r2));
	__asm("mov %0, r2" : "=r" (ret));
	return *((void **)ret);
}

int execve(const char *file, char * const argv[], char * const envp[])
{
	volatile uint32_t ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)file;
	register uint32_t r2 __asm("r2") = (uint32_t)argv;
	register uint32_t r3 __asm("r3") = (uint32_t)envp;
	register uint32_t r12 __asm("r12") = (uint32_t)&ret;
	__asm("\
		mov r0, 5; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		mov r12, %3; \
		svc 0; \
	" :: "r" (r1), "r" (r2), "r" (r3), "r" (r12));

	if (ret == (uint32_t)-1)
		return ret;

	((void (*)(void))ret)();
}

//
// Clock-related calls

void delay(unsigned int ms)
{
	register uint32_t r1 __asm("r1") = ms;
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		svc 2; \
	" :: "r" (r1));
}

unsigned int ticks(void)
{
	unsigned int ret = 0;
	register uint32_t r1 __asm("r1") = (uint32_t)&ret;
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		svc 2; \
	" :: "r" (r1));
	return ret;
}

//
// File-related calls

int mount(vfs_volume_funcs *funcs, uint32_t flags)
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

int open(const char *path, uint32_t flags)
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

int close(int fd)
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
	return ret;
}

int read(int fd, uint32_t count, uint8_t *buffer)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = fd;
	register uint32_t r2 __asm("r2") = count;
	register uint32_t r3 __asm("r3") = (uint32_t)buffer;
	register uint32_t r12 __asm("r12") = (uint32_t)&ret;
	__asm("\
		mov r0, 3; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		mov r12, %3; \
		svc 3; \
	" :: "r" (r1), "r" (r2), "r" (r3), "r" (r12));
	return ret;
}

int write(int fd, uint32_t count, const uint8_t *buffer)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = fd;
	register uint32_t r2 __asm("r2") = count;
	register uint32_t r3 __asm("r3") = (uint32_t)buffer;
	register uint32_t r12 __asm("r12") = (uint32_t)&ret;
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		mov r3, %2; \
		mov r12, %3; \
		svc 3; \
	" :: "r" (r1), "r" (r2), "r" (r3), "r" (r12));
	return ret;
}

