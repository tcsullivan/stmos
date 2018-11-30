/**
 * @file clock.h
 * Clock utilities
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

#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

/**
 * Initializes clocks, setting HCLK (system clock) to 80MHz, the maximum.
 */
extern void clock_init(void);

/**
 * Sleeps for given amount of milliseconds.
 * @param ms Number of milliseconds to sleep for
 */
void clock_delay(uint32_t ms);

/**
 * Sleeps for the given amount of microseconds.
 * @param count Number of microseconds to sleep for
 */
void clock_udelay(uint32_t count);

/**
 * Returns the number of milliseconds since clock initialization.
 * @return milliseconds since init
 */
uint32_t clock_millis(void);

#endif // CLOCK_H_
