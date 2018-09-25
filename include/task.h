/**
 * @file task.h
 * Provides multitasking functionality
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

#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

/**
 * A structure to contain task data.
 */
typedef struct {
	void *next;      /**< pointer to the next task_t instance */
	uint32_t *stack; /**< pointer to the task's stack */
	uint32_t *sp;    /**< pointer to the task's last sp register value */
} task_t;

/**
 * Enters multitasking mode. The given function acts as the initial thread.
 * This task is given a 4kb stack.
 * @param init the initial thread to run
 */
void task_init(void (*init)(void));

/**
 * Starts a new task.
 * @param task the code to run
 * @param stackSize how many bytes of stack to give the thread
 */
void task_start(void (*task)(void), uint16_t stackSize);

/**
 * Allows task switching to be disabled, for low-level actions.
 * Multiple holds can be placed, and all must be removed to continue task
 * switching.
 * @param hold non-zero for hold, zero to remove hold
 */
void task_hold(uint8_t hold);

void _exit(int code);

//int fork(void);

#endif // TASK_H_
