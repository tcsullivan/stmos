#include "gpio.h"

#include <stdint.h>

void gpioMode(gpio_pin_t pin, int mode)
{
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = mode;
	__asm("\
		mov r0, 0; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
}

void gpioType(gpio_pin_t pin, int type)
{
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = type;
	__asm("\
		mov r0, 1; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
}

void gpioPuPd(gpio_pin_t pin, int pupd)
{
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = pupd;
	__asm("\
		mov r0, 2; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
}

void gpioSpeed(gpio_pin_t pin, int speed)
{
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = speed;
	__asm("\
		mov r0, 3; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
}

void gpioWrite(gpio_pin_t pin, int value)
{
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = value;
	__asm("\
		mov r0, 4; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
}

int gpioRead(gpio_pin_t pin)
{
	int ret = 0;
	register uint32_t r1 __asm("r1") = pin;
	register uint32_t r2 __asm("r2") = (uint32_t)&ret;
	__asm("\
		mov r0, 5; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (r1), "r" (r2));
	return ret;
}

