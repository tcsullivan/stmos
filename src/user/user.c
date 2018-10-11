#include <kernel/clock.h>
#include "priv_gpio.h"
#include <kernel/task.h>

void task1(void);
void task2(void);

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
	task_start(task1, 512);

	for (int i = 0; i < 8; i++) {
		gpio(GPIO_OUT, 5, !(i & 1));
		user_delay(200);
	}
}

void task1(void)
{
	user_delay(400);
	task_start(task2, 1024);
}

void task2(void)
{
	int state = 0;
	user_delay(2500);
	while (1) {
		gpio(GPIO_OUT, 5, state ^= 1);
		user_delay(500);
	}
}
