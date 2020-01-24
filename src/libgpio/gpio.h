/**
 * @file gpio.h
 * Userspace library for GPIO access
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

#ifndef STMOS_GPIO_H_ 
#define STMOS_GPIO_H_

#define GP(a) ((a - 'A') << 4)

/**
 * Defines possible modes for a gpio pin.
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


typedef unsigned int gpio_pin_t;

void gpioMode(gpio_pin_t pin, int mode);
void gpioType(gpio_pin_t pin, int type);
void gpioPuPd(gpio_pin_t pin, int pupd);
void gpioSpeed(gpio_pin_t pin, int speed);
void gpioWrite(gpio_pin_t pin, int value);
int gpioRead(gpio_pin_t pin);

#endif // STMOS_GPIO_H_

