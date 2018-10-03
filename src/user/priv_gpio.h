/**
 * @file priv_gpio.h
 * GPIO access for unpriviledged processes
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

#ifndef PRIV_GPIO_H_
#define PRIV_GPIO_H_

// For value flags
#include <kernel/gpio.h>

/**
 * Possible GPIO calls.
 */
enum GPIO_CALLS {
	GPIO_MODE = 0, /**< Change GPIO mode */
	GPIO_TYPE, /**< Change GPIO type (PuPd/ODrain) */
	GPIO_PUPD, /**< Set/clear pullup/pulldown */
	GPIO_SPEED, /**< Set GPIO speed */
	GPIO_OUT, /**< Set GPIO output state */
};

/**
 * Provides unpriviledged GPIO access.
 * @param call The type of GPIO call to make
 * @param pin The pin to modify (0-15 = A, 16-31 = B, ...)
 * @param value The value to pass to the call
 */
void gpio(uint32_t call, uint32_t pin, uint32_t value) 
{
	register uint32_t r0 asm("r0") = call;
	register uint32_t r1 asm("r1") = pin;
	register uint32_t r2 asm("r2") = value;

	asm("\
		mov r0, %0; \
		mov r1, %1; \
		mov r2, %2; \
		svc 1; \
	" :: "r" (r0), "r" (r1), "r" (r2));
}

#endif // PRIV_GPIO_H_
