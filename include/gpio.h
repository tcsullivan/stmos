/**
 * @file gpio.h
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

#ifndef GPIO_H_
#define GPIO_H_

#include <stm32l476xx.h>

/**
 * Helps simplify gpio calls.
 * @param p port, e.g. GPIOA
 * @param b pin, e.g. 4
 */
#define GPIO_PORT(p, b) GPIO##p, b

/**
 * Defines possible modes for a gpio pin
 */
enum GPIO_MODE
{
	INPUT = 0, /**< digital input */
	OUTPUT,    /**< digital output */
	ALTERNATE, /**< alternate function */
	ANALOG     /**< analog function */
};

/**
 * Defines whether to use push-pull or open drain.
 */
enum GPIO_TYPE
{
	PUSHPULL = 0, /**< push-pull */
	OPENDRAIN     /**< open drain */
};

/**
 * Defines the pin's speed
 */
enum GPIO_SPEED
{
	LOW = 0, /**< low */
	MEDIUM,  /**< medium */
	HIGH,    /**< high */
	VERYHIGH /**< very high/maximum */
};

/**
 * Defines if a pullup or pulldown should be used.
 */
enum GPIO_PUPD
{
	NOPUPD,  /**< no pullup/pulldown */
	PULLUP,  /**< use pullup */
	PULLDOWN /**< use pulldown */
};

/**
 * Initializes the gpio.
 */
void gpio_init(void);

/**
 * Enables or disables pullup/pulldown for the given pin.
 * @param port the port, e.g. GPIOA
 * @param pin the pin
 * @param pupd pullup/pulldown enable
 * @see GPIO_PUPD
 */
void gpio_pupd(GPIO_TypeDef *port, uint32_t pin, uint32_t pupd);

/**
 * Sets whether to use push-pull or open drain for the given pin.
 * @param port the port
 * @param pin the pin
 * @param type what to use
 * @see GPIO_TYPE
 */
void gpio_type(GPIO_TypeDef *port, uint32_t pin, uint32_t type);

/**
 * Sets the pin's speed.
 * @param port the port
 * @param pin the pin
 * @param speed the speed to use
 * @see GPIO_SPEED
 */
void gpio_speed(GPIO_TypeDef *port, uint32_t pin, uint32_t speed);

/**
 * Sets the pin's i/o mode.
 * @param port the port
 * @param pin the pin
 * @param mode the mode to use
 * @see GPIO_MODE
 */
void gpio_mode(GPIO_TypeDef *port, uint32_t pin, uint32_t mode);

/**
 * Sets the state of a digital output pin.
 * @param port the port
 * @param pin the pin
 * @param val non-zero for high, zero for low
 */
void gpio_dout(GPIO_TypeDef *port, uint32_t pin, uint32_t val);

/**
 * Reads a digital input pin.
 * @param port the port
 * @param pin the pin
 * @return non-zero for high, zero for low
 */
uint32_t gpio_din(GPIO_TypeDef *port, uint32_t pin);

#endif // GPIO_H_
