/**
 * @file serial.h
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

#ifndef SERIAL_H_
#define SERIAL_H_

/**
 * Initializes the serial device.
 */
void serial_init(uint32_t baud);

/**
 * Puts the given character through serial.
 * @param c the character to send
 */
void serial_put(int c);

/**
 * Gets a character from serial.
 * @return the character
 */
char serial_get(void);

/**
 * Gets a string from serial, cut off by a newline.
 * @param buf the initialized buffer to fill
 * @param max the max amount of bytes to write to the buffer
 */
void serial_gets(char *buf, int max);

#endif // SERIAL_H_

