/**
 * @file clock.c
 * Basic clock utilities
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
#include <stm32l476xx.h>

// ticks since init
volatile uint32_t ticks = 0;

void clock_init(void)
{
	// turn on HSI (16MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

	// get PLLR to 80MHz (max)
	// VCO = C * (N/M) -> 16 * (10/1) = 160
	// SCLK = VCO / R = 160 / 2 = 80 MHz
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 10 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR | RCC_PLLCFGR_PLLQ); // /2
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLQEN;

	// start PLL
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	// set system clock to PLL
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk);
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	// SysTick init. 80MHz / 80000 = 1kHz, ms precision
	SysTick->LOAD = 80000;
	SysTick->CTRL |= 0x07; // no div, interrupt, enable
}

void delay(uint32_t count)
{
	uint32_t target = ticks + count;
	while (ticks < target);
}

void SysTick_Handler(void)
{
	// just keep counting
	ticks++;

	if (!(ticks & 3))
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

