/**
 * @file gpio.c
 * Abstracts gpio access, makes things easier
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

#include <gpio.h>

static const GPIO_TypeDef *gpio_ports[8] = {
	GPIOA, GPIOB, GPIOC, GPIOD,
	GPIOE, GPIOF, GPIOG, GPIOH
};

void gpio_svc(uint32_t *args)
{
	GPIO_TypeDef *port = gpio_ports[args[1] / 16];
	uint32_t pin = args[1] & 0xF;

	switch (args[0]) {
	case 0:
		gpio_mode(port, pin, args[2]);
		break;
	case 1:
		gpio_type(port, pin, args[2]);
		break;
	case 2:
		gpio_pupd(port, pin, args[2]);
		break;
	case 3:
		gpio_speed(port, pin, args[2]);
		break;
	case 4:
		gpio_dout(port, pin, args[2]);
		break;
	}
}

void gpio_init(void)
{
	// enable clocks
	RCC->AHB2ENR |= 0x0F;
}

void gpio_pupd(GPIO_TypeDef *port, uint32_t pin, uint32_t pupd)
{
	port->PUPDR &= ~(0x03 << (2 * pin));
	port->PUPDR |= pupd << (2 * pin);
}

void gpio_speed(GPIO_TypeDef *port, uint32_t pin, uint32_t speed)
{
	port->OSPEEDR &= ~(0x03 << (2 * pin));
	port->OSPEEDR |= speed << (2 * pin);
}

void gpio_type(GPIO_TypeDef *port, uint32_t pin, uint32_t type)
{
	port->OTYPER &= ~(1 << pin);
	port->OTYPER |= type << pin;
}

void gpio_mode(GPIO_TypeDef *port, uint32_t pin, uint32_t mode)
{
	port->MODER &= ~(0x03 << (2 * pin));
	port->MODER |= mode << (2 * pin);
}

void gpio_dout(GPIO_TypeDef *port, uint32_t pin, uint32_t val)
{
	port->BSRR |= (1 << (val ? pin : pin + 16));
}

uint32_t gpio_din(GPIO_TypeDef *port, uint32_t pin)
{
	return port->IDR & (1 << pin);
}
