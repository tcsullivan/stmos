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

#include "heap.h"

// Defines size of alignment on memory allocations
#define HEAP_ALIGN 4

// Linked list of free'd memory
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
    // Count free'd block sizes
	for (alloc_t *node = free_blocks; node != 0; node = node->next)
		total += node->size;
    // Add remaining free memory to that total
	return total + (0x20018000 - (uint32_t)heap_end);
}

void *malloc(uint32_t size)
{
	if (size == 0)
		return 0;

    // Round size to an aligned value
	size = (size + sizeof(alloc_t) + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);

    // Begin searching through free'd blocks to see if we can reuse some memory.
	alloc_t *node = free_blocks;
	alloc_t *prev = 0;
	while (node != 0) {
		if (node->size >= size) {
			// If we can use this chunk, remove it from the free_blocks chain
			if (prev != 0)
				prev->next = node->next;
			else
				free_blocks = node->next;
			node->next = 0;

			// If this chunk is really big, give back the extra space
			if (node->size > size + 64) { // TODO why 64..?
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

    // No reusable chunks, take from the end of the heap
	node = (alloc_t *)heap_end;
	node->size = size;
	node->next = 0;

	heap_end = (void *)((uint8_t *)heap_end + size);

	return (void *)((uint8_t *)node + sizeof(alloc_t)); 
}

void *calloc(uint32_t count, uint32_t size)
{
    // Simply malloc and zero
	uint8_t *buf = malloc(count * size);
	for (uint32_t i = 0; i < count * size; i++)
		buf[i] = 0; // TODO safe?
	return buf;
}

void free(void *buf)
{
	if (buf == 0)
		return;

    // Get the alloc_t structure of this chunk
	alloc_t *alloc = (alloc_t *)((uint8_t *)buf - sizeof(alloc_t));
	if (alloc->next != 0)
		return;

    // Search through the free_blocks list to see if this free chunk can merge
    // into an adjacent free chunk.
	int merged = 0;
	for (alloc_t *prev = 0, *node = free_blocks; node != 0; prev = node, node = node->next) {
        // If the node after the current one is ours
		if ((uint32_t)node + sizeof(alloc_t) + node->size == (uint32_t)alloc) {
            // Merge by adding our node's size to this one
			merged |= 1;
			node->size += sizeof(alloc_t) + alloc->size;
			break;
		}
        // Or, if this current node is the one after ours
        else if ((uint32_t)buf + alloc->size == (uint32_t)node) {
            // Merge the current node into ours
			merged |= 1;
			alloc->size += sizeof(alloc_t) + node->size;
			alloc->next = node->next;

            // Take the current node's place in the free_blocks chain
			if (prev != 0)
				prev->next = alloc;
			else
				free_blocks = alloc;
			break;
		}
	}

    // If we couldn't merge, simply append to the chain
	if (merged == 0) {
		alloc->next = free_blocks;
		free_blocks = alloc;
	}

}

