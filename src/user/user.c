#include <kernel/clock.h>
#include "priv_gpio.h"
#include <kernel/task.h>

void task1(void);
void task2(void);



void user_main(void)
{
	gpio(GPIO_MODE, 5, OUTPUT);
	task_start(task1, 512);

	for (int i = 0; i < 8; i++) {
		gpio(GPIO_OUT, 5, !(i & 1));
		delay(200);
	}
}

void task1(void)
{
	delay(400);
	task_start(task2, 1024);
}

void task2(void)
{
	int state = 0;
	delay(2500);
	while (1) {
		gpio(GPIO_OUT, 5, state ^= 1);
		delay(500);
	}
}
