/**
 * @file heap.h
 * A basic memory manager
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

#ifndef HEAP_H_
#define HEAP_H_

#include <stdint.h>

/**
 * Internal structure place before memory allocations, to track allocation size.
 */
typedef struct {
	uint32_t size;
	void *next;
} __attribute__ ((packed)) alloc_t;

/**
 * Initializes memory management of the given heap.
 * Note: Heap size is not accounted for.
 * @param buf The heap to use for allocations
 */
void heap_init(void *buf);

/**
 * Returns the amount of free, allocatable memory, in bytes, assuming that the
 * heap has from its initial location to the end of SRAM.
 * @return Amount of free memory in bytes
 */
uint32_t heap_free(void);

/**
 * Allocates a chunk of memory.
 * @param size How many bytes to claim
 * @return Pointer to the allocated buffer
 */
void *malloc(uint32_t size);

/**
 * Allocates and zeros a chunk of memory.
 * @param count How many of whatever to allocate
 * @param size Byte count of each whatever
 * @return Pointer to the allocated buffer
 */
void *calloc(uint32_t count, uint32_t size);

/**
 * Frees the buffer allocated through malloc/calloc.
 * Please don't double-free.
 * @param The buffer to release
 */
void free(void *buf);

#endif // HEAP_H_

