/**
 * @file heap.c
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

#include <heap.h>

#define HEAP_ALIGN 4

static alloc_t *free_blocks;
static void *heap_end;

void heap_init(void *buf)
{
	heap_end = buf;
	free_blocks = 0;
}

uint32_t heap_free(void)
{
	uint32_t total = 0;
	for (alloc_t *node = free_blocks; node != 0; node = node->next)
		total += node->size;
	return total + (0x20018000 - (uint32_t)heap_end);
}

void *malloc(uint32_t size)
{
	if (size == 0)
		return 0;

	size = (size + sizeof(alloc_t) + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);

	alloc_t *node = free_blocks;
	alloc_t *prev = 0;
	while (node != 0) {
		if (node->size >= size) {
			// get out of the free chain
			if (prev != 0)
				prev->next = node->next;
			else
				free_blocks = node->next;
			node->next = 0;

			// split alloc if too big
			if (node->size > size + 64) {
				alloc_t *leftover = (alloc_t *)((uint32_t)node
					+ sizeof(alloc_t) + size);
				leftover->size = node->size - size - sizeof(alloc_t);
				leftover->next = 0;
				free((uint8_t *)leftover + sizeof(alloc_t));
				node->size = size;
				return (void *)((uint8_t *)node + sizeof(alloc_t));
			}

			return (void *)((uint8_t *)node + sizeof(alloc_t));
		}

		prev = node;
		node = node->next;
	}

	node = (alloc_t *)heap_end;
	node->size = size;
	node->next = 0;

	heap_end = (void *)((uint8_t *)heap_end + size);

	return (void *)((uint8_t *)node + sizeof(alloc_t)); 
}

void *calloc(uint32_t count, uint32_t size)
{
	uint8_t *buf = malloc(count * size);
	for (uint32_t i = 0; i < count * size; i++)
		buf[i] = 0;
	return buf;
}

void free(void *buf)
{
	if (buf == 0)
		return;

	alloc_t *alloc = (alloc_t *)((uint8_t *)buf - sizeof(alloc_t));
	if (alloc->next != 0)
		return;

	// check for adjacent free'd blocks
	int merged = 0;
	for (alloc_t *prev = 0, *node = free_blocks; node != 0; prev = node, node = node->next) {
		if ((uint32_t)node + sizeof(alloc_t) + node->size == (uint32_t)alloc) {
			// block before
			merged |= 1;
			node->size += sizeof(alloc_t) + alloc->size;
			break;
			//alloc = node;
		} else if ((uint32_t)buf + alloc->size == (uint32_t)node) {
			// block after
			merged |= 1;
			alloc->size += sizeof(alloc_t) + node->size;
			alloc->next = node->next;
			if (prev != 0)
				prev->next = alloc;
			else
				free_blocks = alloc;
			break;
		}
	}

	if (merged == 0) {
		alloc->next = free_blocks;
		free_blocks = alloc;
	}

}
