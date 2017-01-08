/*
 * sc.S
 *
 * - 内核的多任务调度
 */

#include <kernel.h>
#include <sc.h>

#define	OFFOF_TASK_SP	4

.sect	.text
.globl	switch_context
.globl	__kernel_thread_starter
.globl	_cpu_halt
.globl	_cpu_idle

switch_context:
	pushl	%ebp	# 1bb4
	pushl	%esi
	pushl	%edi
	pushl	%ebx
	movl	%esp, 4(%ecx)
	movl	4(%edx), %esp
	popl	%ebx
	popl	%edi
	popl	%esi
	popl	%ebp
	ret

__kernel_thread_starter:
	pushl	$kernel_thread_exit
	jmp	*%ebx	/* 'ebx' is given as thread entry in kernel_thread() */

_cpu_halt:
	cli
_cpu_idle:
	hlt
	jmp	_cpu_idle

