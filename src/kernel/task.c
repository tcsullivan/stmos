/**
 * @file task.c
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

#include "clock.h"
#include "heap.h"
#include "task.h"
#include <arch/stm/stm32l476xx.h>

task_t *current, *prev;
static uint8_t task_disable = 0;

int task_fork(uint32_t sp);
void task_svc(uint32_t *args)
{
	int result = task_fork(args[0]);
	args[0] = result;
}

void task_hold(uint8_t hold)
{
	if (hold != 0)
		task_disable++;
	else if (task_disable > 0)
		task_disable--;
}

void _exit(int code)
{
	(void)code;

	while (prev == 0);

	prev->next = current->next;

	// Free this thread's stack, and task data.
	// Since we're single core, no one else can claim this memory until
	// a task switch, after which we're done with this memory anyway.
	free(current->stack);
	free(current);

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}


/**
 * Exits the task (userspace call).
 */
__attribute__ ((naked))
void task_doexit(void)
{
	asm("eor r0, r0; svc 0");
	while (1);
}

/**
 * 'Prepares' task for running.
 * Calls the task's main code, setting task_doexit() (_exit) as the return point.
 */
__attribute__ ((naked))
void task_crt0(void)
{
	asm("\
		mov r4, lr; \
		ldr lr, =task_doexit; \
		bx r4; \
	");
}

task_t *task_create(void (*code)(void), uint16_t stackSize)
{
	task_t *t = (task_t *)malloc(sizeof(task_t));
	t->next = 0;
	t->stack = (uint32_t *)malloc(stackSize);
	void *sp = (uint8_t *)t->stack + stackSize - 68; // excep. stack + regs
	t->sp = sp;

	/*
		sp[0-7]  - r4-r11
		sp[8]    - r14 (lr)
		sp[9-12] - r0-r3
		sp[13]   - r12
		sp[14]   - LR
		sp[15]   - PC
		sp[16]   - xPSR
	*/

	for (uint8_t i = 0; i < 14; i++)
		t->sp[i] = 0;
	t->sp[8] = 0xFFFFFFFD;
	t->sp[14] = (uint32_t)code;
	t->sp[15] = (uint32_t)task_crt0;
	t->sp[16] = 0x01000000;
	t->sleep = 0;
	return t;
}

void task_init(void (*init)(void), uint16_t stackSize)
{
	current = (task_t *)malloc(sizeof(task_t));
	current->stack = 0; // free() is called on this

	task_t *init_task = task_create(init, stackSize);

	prev = init_task;
	current->next = init_task;
	init_task->next = init_task;

	task_disable = 0;

	// bit 0 - priv, bit 1 - psp/msp
	asm("\
		isb; \
		cpsie i; \
		mov r0, sp; \
		msr psp, r0; \
		mrs r0, control; \
		orr r0, r0, #3; \
		msr control, r0; \
	");

	// exit the current (fake) task
	task_doexit();
}

void task_start(void (*task)(void), uint16_t stackSize)
{
	task_hold(1);
	task_t *t = task_create(task, stackSize);
	t->next = current->next;
	current->next = t;
	task_hold(0);
}

int task_fork_ret(void)
{
	return 0;
}

// Return 0 for child, non-zero for parent
int task_fork(uint32_t sp)
{
	task_hold(1);

	// 1. Get a PC for the child
	void (*pc)(void) = (void (*)(void))((uint32_t)task_fork_ret);

	// 2. Prepare child task
	alloc_t *stackInfo = (alloc_t *)(((uint8_t *)current->stack)
		- sizeof(alloc_t));
	task_t *childTask = task_create(pc, stackInfo->size - 8);
	for (uint32_t i = 0; i < (stackInfo->size - 8); i++)
		childTask->stack[i] = current->stack[i];

	//uint32_t *sp;
	//asm("mov %0, sp" : "=r" (sp));
	childTask->sp = (uint32_t *)((uint32_t)childTask->stack + (sp
		- (uint32_t)current->stack));

	// 3. Insert child into task chain
	childTask->next = current->next;
	current->next = childTask;

	// 4. Re-enable scheduler, make change happen
	task_hold(0);

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	return 1;
}

__attribute__ ((naked))
void PendSV_Handler(void) 
{
	if (task_disable != 0)
		asm("bx lr");

	// TODO get back to c, implement task sleeping

	// Save current stack pointer
	asm("\
		mrs r0, psp; \
		isb; \
		stmdb r0!, {r4-r11, r14}; \
		mov %0, r0; \
	" : "=r" (current->sp));

	// Load next task
	prev = current;
	uint32_t ticks = millis();
	do {
		current = current->next;
	} while (current->sleep > ticks);
	current->sleep = 0;

	// Load stack pointer, return
	asm("\
		mov r0, %0; \
		ldmia r0!, {r4-r11, r14}; \
		msr psp, r0; \
		bx lr; \
	" :: "r" (current->sp));
}

