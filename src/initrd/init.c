/**
 * @file test.c
 * Basic userland code to be loaded by stmos for program load testing.
 */

#include <stdio.h>
#include <gpio.h>
#include <syscalls.h>

int main(void)
{
	gpioMode(5, OUTPUT);

	printf("Hello, world!\n");

	while (1) {
		gpioWrite(5, 1);
		delay(1000);
		gpioWrite(5, 0);
		delay(500);
	}
	return 0;
}

