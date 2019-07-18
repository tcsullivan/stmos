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

#include "clock.h"
#include "task.h"
#include <arch/stm/stm32l476xx.h>

// ticks since init
volatile uint32_t clock_ticks = 0;

volatile uint8_t tim2_finished = 1;

void clock_svc(uint32_t n, uint32_t *ret, uint32_t *args)
{
	if (n == 0)
		task_sleep(args[0]);
	else if (n == 1)
		clock_udelay(args[0]);
	else if (n == 2)
		*((unsigned int *)ret) = clock_millis();
}

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

	// Prepare TIM2 for microsecond timing
	NVIC_EnableIRQ(TIM2_IRQn);
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->PSC = 40 - 1;
	TIM2->ARR = 100;
	TIM2->CR1 |= TIM_CR1_OPM | TIM_CR1_CEN;
}

void clock_delay(uint32_t count)
{
	uint32_t target = clock_ticks + count;
	while (clock_ticks < target);
}

void clock_udelay(uint32_t count)
{
	tim2_finished = 0;
	TIM2->ARR = count;
	TIM2->CR1 |= TIM_CR1_CEN;
	while (tim2_finished == 0);
}

uint32_t clock_millis(void)
{
	return clock_ticks;
}

void SysTick_Handler(void)
{
	// just keep counting
	clock_ticks++;

	// task switch every four ticks (4ms)
	if (!(clock_ticks & 3))
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~(TIM_SR_UIF);
	tim2_finished |= 1;
}

