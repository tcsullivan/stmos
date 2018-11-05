/**
 * @file init.c
 * Kernel initialization procedure
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

#include "clock.h"
#include "gpio.h"
#include "heap.h"
#include "task.h"
#include "vfs.h"
#include <fs/initrd.h>
#include <arch/stm/stm32l476xx.h>

extern uint8_t __bss_end__;

extern void user_main(void);
void init_idle(void);

int main(void)
{
	asm("cpsid i");
	// disable cached writes for precise debug info
	//*((uint32_t *)0xE000E008) |= 2;

	// prepare flash latency for 80MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	// init core components
	clock_init();
	heap_init(&__bss_end__);
	gpio_init();

	vfs_init();
	initrd_init();

	// enable FPU
	//SCB->CPACR |= (0xF << 20);

	task_init(init_idle, 512);
	while (1);
}

void init_idle(void)
{
	task_start(user_main, 4096);

	while (1)
		delay(10);
}
