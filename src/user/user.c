#include "priv_gpio.h"

#include <kernel/clock.h>
#include <kernel/heap.h>
#include <kernel/vfs.h>

#include "syscalls.h"

void user_delay(uint32_t ms)
{
	register uint32_t r1 asm("r1") = ms;

	asm("\
		mov r0, 0; \
		mov r1, %0; \
		svc 2; \
	" :: "r" (r1));
}

void user_main(void)
{
	gpio(GPIO_MODE, 5, OUTPUT);

	int test = vfs_open("A:/hello", VFS_FILE_READ);
	char *buf = malloc(20);
	int count = vfs_read(test, 20, (uint8_t *)buf);
	(void)count;

	if (fork() == 0) {
		while (1) {
			gpio(GPIO_OUT, 5, 1);
			user_delay(2000);
		}
	} else {
		while (1) {
			user_delay(1000);
			gpio(GPIO_OUT, 5, 0);
			user_delay(1000);
		}
	}
}

