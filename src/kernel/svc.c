/**
 * @file svc.c
 * Handler code for SVC calls
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

#include <stdint.h>
#include "gpio.h"
#include "clock.h"
#include "task.h"

extern void gpio_svc(uint32_t, uint32_t *, uint32_t *);
extern void clock_svc(uint32_t, uint32_t *, uint32_t *);
extern void task_svc(uint32_t, uint32_t *, uint32_t *);
extern void vfs_svc(uint32_t, uint32_t *, uint32_t *args);

void SVC_Handler(void) {
	uint32_t *stack;

    // Get the appropriate stack (either kernel or process)
	asm("\
		tst lr, #4; \
		ite eq; \
		mrseq %0, msp; \
		mrsne %0, psp; \
	" : "=r" (stack));

	int svc_number = ((char *)stack[6])[-2];
	uint32_t min_number = stack[0];
	uint32_t *ret = (uint32_t *)stack[1];
	uint32_t *args = (uint32_t *)stack[2];

	switch (svc_number) {
	case -1: // TODO why?
	case 0: /* Task-related calls
		 * 0 - _exit
		 * 1 - fork
		 * 2 - getpid
		 * 3 - waitpid
		 * 4 - sbrk (TODO poor implementation)
		 * 5 - execve
		 */
		task_svc(min_number, ret, args);
		break;

	case 1: /* GPIO-related calls
		 * 0 - gpio_mode
		 * 1 - gpio_type
		 * 2 - gpio_pupd
		 * 3 - gpio_speed
		 * 4 - gpio_dout
		 * 5 - gpio_din
		 */
		gpio_svc(min_number, ret, args);
		break;

	case 2: /* Clock-related calls
		 * 0 - delay
		 * 1 - udelay
		 * 2 - ticks
		 */
		clock_svc(min_number, ret, args);
		break;

	case 3: /* Filesystem-related calls
		 * 0 - mount
		 * 1 - open
		 * 2 - close
		 * 3 - read
		 * 4 - write
		 */
		vfs_svc(min_number, ret, args);
		break;
	default:
		break;
	}
}

