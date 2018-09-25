/**
 * @file main.c
 * Entry point for operating system
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

#include <clock.h>
#include <gpio.h>
#include <heap.h>
#include <stm32l476xx.h>
#include <task.h>

extern uint8_t __bss_end__;

void kmain(void);

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

	// enable FPU
	//SCB->CPACR |= (0xF << 20);

	task_init(kmain);
	while (1);
}

void task2(void);
void kmain(void)
{
	gpio_mode(GPIOA, 5, OUTPUT);
	task_start(task2, 512);

	for (int i = 0; i < 8; i++) {
		gpio_dout(GPIOA, 5, !(i & 1));
		delay(200);
	}

	return;
}

void task2(void)
{
	while (1)
		delay(800);
}
