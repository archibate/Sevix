/*
 * head.S
 *
 * - head of the kernel for vexpress-a15
 *
 * CROSS=/opt/FriendlyARM/toolschain/4.4.3/bin/arm-linux-
 */

#include <kernel.h>
#include <init.h>
#include <mm.h>
#include <iomem.h>

.sect	.boot.text
.globl	__kernel_entry

.equ	boot_pd, 0x80000000

__kernel_entry:
	cpsid	if
	cps	#0x13
	ldr	sp, =__boot_stack_top
	stmfd	sp!, {r0-r12}
	ldr	r0, =0x53000000
	mov	r1, #0
	str	r1, [r0]
	ldr	r4, =boot_pd
	stmfd	sp!, {r4-r5}
	add	r5, r4, #ENTS_PER_PD * 4
	mov	r0, #0
	mov	r1, r0
	mov	r2, r0
	mov	r3, r0
1:	stmia	r4!, {r0-r3}
	cmp	r4, r5
	blt	1b
	ldmfd	sp!, {r4-r5}
	ldr	r0, =(KERNEL_LOADED_PADDR & 0xFFF00000) \
		| MMU_SPECIAL | MMU_SECTION | MMU_WRITEBACK | MMU_MANAGER
	ldr	r1, =(KERNEL_LOADED_PADDR >> 20) * 4
	ldr	r2, =(KPHY2LIN(KERNEL_LOADED_PADDR) >> 20) * 4
	str	r0, [r4, r1]
	str	r0, [r4, r2]
	add	r0, r0, #4
	add	r1, r1, #4
	add	r2, r2, #4
	str	r0, [r4, r1]
	str	r0, [r4, r2]
	add	r0, r0, #4
	add	r1, r1, #4
	add	r2, r2, #4
	str	r0, [r4, r1]
	str	r0, [r4, r2]
	add	r0, r0, #4
	add	r1, r1, #4
	add	r2, r2, #4
	str	r0, [r4, r1]
	str	r0, [r4, r2]
	ldr	r0, =(IOMEM_BASE_PADDR & 0xFFF00000) \
		| MMU_SPECIAL | MMU_SECTION | MMU_MANAGER
	ldr	r1, =(IOMEM_BASE_VADDR >> 20) * 4
	str	r0, [r4, r1]
	mcr	p15, 0, r0, c7, c7, 0
	mcr	p15, 0, r0, c7, c10, 4
	mcr	p15, 0, r0, c8, c7, 0
	mcr	p15, 0, r4, c2, c0, 0
	@mvn	r0, #0x0
	ldr	r0, =0b110100
	mcr	p15, 0, r0, c3, c0, 0
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x7A00
	orr	r0, r0, #0x008F
	bic	r0, r0, #0x8000
	bic	r0, r0, #0x0010
	mcr	p15, 0, r0, c1, c0, 0
	cpsie	f
	ldmfd	sp, {r0-r3}	/* U-Boot args */
	ldr	lr, =_cpu_idle
	ldr	pc, =start_kernel

.ltorg

.sect	.boot.bss
	.space	1024, 0xCC
__boot_stack_top:

.sect	.text
.globl	_cpu_halt, _cpu_idle
_cpu_halt:
	cpsid	if
_cpu_idle:
	wfi
	b	_cpu_idle
	@;cpsid i;1:wfi;b 1b

.ltorg

