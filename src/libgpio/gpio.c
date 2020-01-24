/**
 * @file gpio.c
 * Userspace library for GPIO access
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "gpio.h"

#include <stdint.h>

// SVC calls get messed up by GCC's optimization...
#define NOOPTIMIZE __attribute__((optimize(0)))

NOOPTIMIZE
void gpioMode(gpio_pin_t pin, int mode)
{
	uint32_t args[2] = { (uint32_t)pin, mode };
	__asm volatile("\
		mov r0, 0; \
		mov r1, 0; \
		mov r2, %0; \
		svc 1; \
	" :: "r" (args));
}

NOOPTIMIZE
void gpioType(gpio_pin_t pin, int type)
{
	uint32_t args[2] = { (uint32_t)pin, type };
	__asm volatile("\
		mov r0, 1; \
		mov r1, 0; \
		mov r2, %0; \
		svc 1; \
	" :: "r" (args));
}

NOOPTIMIZE
void gpioPuPd(gpio_pin_t pin, int pupd)
{
	uint32_t args[2] = { (uint32_t)pin, pupd };
	__asm volatile("\
		mov r0, 2; \
		mov r1, 0; \
		mov r2, %0; \
		svc 1; \
	" :: "r" (args));
}

NOOPTIMIZE
void gpioSpeed(gpio_pin_t pin, int speed)
{
	uint32_t args[2] = { (uint32_t)pin, speed };
	__asm volatile("\
		mov r0, 3; \
		mov r1, 0; \
		mov r2, %0; \
		svc 1; \
	" :: "r" (args));
}

NOOPTIMIZE
void gpioWrite(gpio_pin_t pin, int value)
{
	uint32_t args[2] = { (uint32_t)pin, value };
	__asm volatile("\
		mov r0, 4; \
		mov r1, 0; \
		mov r2, %0; \
		svc 1; \
	" :: "r" (args));
}

NOOPTIMIZE
int gpioRead(gpio_pin_t pin)
{
	volatile int ret = 0;
	uint32_t args[1] = { (uint32_t)pin };
	__asm volatile("\
		mov r0, 5; \
		mov r1, %0; \
		mov r2, %1; \
		svc 1; \
	" :: "r" (&ret), "r" (args));
	return ret;
}

