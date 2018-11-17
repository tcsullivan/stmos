/**
 * @file serial.c
 * Provides basic serial IO (through STM debug stuff)
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

#include <arch/stm/stm32l476xx.h>
#include "gpio.h"
#include "clock.h"

void serial_init(uint32_t baud)
{
	gpio_mode(GPIOA, 2, ALTERNATE);
	gpio_mode(GPIOA, 3, ALTERNATE);
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |= 0x00007700;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// start usart device
	USART2->BRR = 80000000L / baud;
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void serial_put(int c)
{
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = c & 0xFF;
}

char serial_get(void)
{
	while (!(USART2->ISR & USART_ISR_RXNE))
		delay(10);
	return USART2->RDR & 0xFF;
}

void serial_gets(char *buf, int max)
{
	uint16_t index = 0;

	do {
		buf[index] = serial_get();
		serial_put(buf[index]);
	} while (buf[index] != '\r' && index++ < max);
	buf[index] = '\0';
}
