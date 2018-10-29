#include <kernel/clock.h>
#include "priv_gpio.h"
#include <kernel/task.h>

void user_delay(uint32_t ms)
{
	register uint32_t r1 asm("r1") = ms;

	asm("\
		mov r0, 0; \
		mov r1, %0; \
		svc 2; \
	" :: "r" (r1));
}

int fork(void)
{
	int result = 0;
	asm("\
		mov r0, 1; \
		mov r1, %0; \
		svc 0; \
	" :: "r" (&result));
	return result;
}

void user_main(void)
{
	gpio(GPIO_MODE, 5, OUTPUT);

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

