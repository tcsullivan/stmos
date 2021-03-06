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

#define WIFEXITED(w)   (w & (1 << 8))
#define WEXITSTATUS(w) (w & 0xFF)
typedef uint16_t pid_t;

/**
 * A structure to contain task data.
 */
typedef struct task_t {
	struct task_t *next; /**< pointer to the next task_t instance */
	uint32_t *sp;        /**< pointer to the task's last sp register value */
	uint32_t *stack;     /**< pointer to the task's stack */
	uint32_t *heap;
	pid_t pid;           /**< Task (Process) ID */
	pid_t pgid;          /**< Process Group ID */
	struct {
		uint32_t state : 8;
		uint32_t value : 24;
	} status;
} task_t;

enum TASK_STATUS_FLAGS {
	TASK_RUNNING,  /**< Task is actively running */
	TASK_SLEEPING, /**< Task is sleeping for task_t.sleep ms */
	TASK_EXITED,   /**< Task has exited, task_t.sleep has code */
	TASK_ZOMBIE    /**< Task exited, accounted for, ready to go bye bye */
};

/**
 * Enters multitasking mode. The given function acts as the initial thread.
 * This task is given a 4kb stack.
 * @param init the initial thread to run
 */
void task_init(void (*init)(void), uint16_t stackSize);

/**
 * Starts a new task.
 * @param task the code to run
 * @param stackSize how many bytes of stack to give the thread
 */
void task_start(void (*task)(void), uint16_t stackSize);

/**
 * Allows task switching to be disabled, for low-level/time-critical  operations.
 * Multiple holds can be placed, and all must be removed to continue task
 * switching.
 * @param hold non-zero for hold, zero to remove hold
 */
void task_hold(uint8_t hold);

/**
 * Puts the task to sleep for the given duration.
 * @param ms Milliseconds to sleep for
 */
void task_sleep(uint32_t ms);

/**
 * Called by the task when it's ready to exit.
 * @param code Task's exit code
 */
void task_exit(int code);

/**
 * Forks the task into two processes.
 */
int task_fork(void);

/**
 * Gets the task's PID.
 */
pid_t task_getpid(void);

/**
 * Waits for a task to change state (TODO confirm).
 */
pid_t task_waitpid(pid_t pid, int *wstatus, int options);

/**
 * TODO
 */
void *task_sbrk(uint32_t bytes);

#endif // TASK_H_

