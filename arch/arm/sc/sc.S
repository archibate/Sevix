/*
 * sc.S
 *
 * - 内核的多任务调度
 */

#include <kernel.h>
#include <sc.h>
#include <mm.h>
#include <init.h>

#define	OFFOF_TASK_SP	4

.sect	.text
.globl	switch_context
.globl	__kernel_thread_starter

switch_context:
	stmfd	sp!, {r4-r12, lr}
1:	str	sp, [r0, #OFFOF_TASK_SP]
2:	ldr	sp, [r1, #OFFOF_TASK_SP]
	ldmfd	sp!, {r4-r12, pc}

__kernel_thread_starter:
	ldr	lr, =kernel_thread_exit
	bx	r4	/* 'r4' is given as thread entry in kernel_thread() */

