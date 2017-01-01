/*
 * sc.S
 *
 * - 内核的多任务调度
 */

#include <kernel.h>
#include <init.h>
#include <mm.h>
#include <iomem.h>

#define	OFFOF_TASK_SP	4

.sect	.text
.globl	switch_context

switch_context:
	stmfd	sp!, {r4-r12, lr}
	str	sp, [r0, #OFFOF_TASK_SP]
	ldr	sp, [r1, #OFFOF_TASK_SP]
	ldmfd	sp!, {r4-r12, pc}

