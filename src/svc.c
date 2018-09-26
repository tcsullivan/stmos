/**
 * @file svc.c
 * An unused handler for SVC calls
 * TODO: use SVC calls, possibly allowing for switch to unprivileged mode?
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
#include <gpio.h>
#include <clock.h>
#include <task.h>

extern void gpio_svc(uint32_t *);

void svc_handler(uint32_t *args)
{
	/*uint32_t*/int svc_number = ((char *)args[6])[-2];

	switch (svc_number) {
	case -1:
	case 0:
		_exit(args[0]);
		break;
	case 1:
		gpio_svc(args);
		break;
	default:
		break;
	}
}

void SVC_Handler(void) {
	uint32_t *args;

	asm("\
		tst lr, #4; \
		ite eq; \
		mrseq %0, msp; \
		mrsne %0, psp; \
	" : "=r" (args));

	svc_handler(args);
}

