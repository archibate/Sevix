/*
 * head.S
 *
 * - 内核入口点
 *
 */

#include <kernel.h>
#include <init.h>
#include <memap.h>
#include <mm.h>
#include <asm/iomem.h>

#define	BOOT_PGDIR_PADDR	0x80000000

.sect	.boot.text
.globl	__kernel_entry

__kernel_entry:
	cpsid	if
	cps	#0x13
	ldr	sp, =__boot_stack_top
	stmfd	sp!, {r0-r12}
	ldr	r0, =0x53000000
	mov	r1, #0
	str	r1, [r0]	/* disable watchdog */
	ldr	r4, =BOOT_PGDIR_PADDR
	stmfd	sp!, {r4-r5}
	add	r5, r4, #PGDIR_ENTS * 4
	mov	r0, #0
	mov	r1, r0
	mov	r2, r0
	mov	r3, r0
1:	stmia	r4!, {r0-r3}
	cmp	r4, r5
	blt	1b
	ldmfd	sp!, {r4-r5}
	ldr	r0, =(KBASE_PADDR & 0xFFF00000) \
		| MMU_SPECIAL | MMU_SECTION | MMU_WRITEBACK | MMU_MANAGER	@ sect_desc
	ldr	r1, =(KBASE >> 20) * 4	@ index
	ldr	r2, =(KBASE_PADDR >> 20) * 4	@ phy_index
	ldr	r3, =(KBASE_END >> 20) * 4
1:	str	r0, [r4, r1]
	str	r0, [r4, r2]
	add	r0, r0, #SECTION_SIZE
	add	r1, r1, #4
	add	r2, r2, #4
	cmp	r1, r3
	blt	1b
	ldr	r0, =(IOMEM_PADDR & 0xFFF00000) \
		| MMU_SPECIAL | MMU_SECTION | MMU_MANAGER	@ sect_desc
	ldr	r1, =(IOMEM_VADDR >> 20) * 4	@ index
	ldr	r3, =(IOMEM_END_VADDR >> 20) * 4	@ BUG?no
1:	str	r0, [r4, r1]
	add	r0, r0, #SECTION_SIZE
	add	r1, r1, #4
	cmp	r1, r3
	blt	1b
	mcr	p15, 0, r0, c7, c7, 0
	mcr	p15, 0, r0, c7, c10, 4
	mcr	p15, 0, r0, c8, c7, 0
	mcr	p15, 0, r4, c2, c0, 0
	ldr	r0, =0b110100
	mcr	p15, 0, r0, c3, c0, 0
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x7A00
	orr	r0, r0, #0x008F
	bic	r0, r0, #0x8000
	bic	r0, r0, #0x0010
	mcr	p15, 0, r0, c1, c0, 0
	ldmfd	sp, {r0-r3}	/* U-Boot args */
	ldr	pc, =vmem_entry

.ltorg

.sect	.boot.bss
	.align	4
	.space	1024, 0xCC
__boot_stack_top:

.sect	.text
vmem_entry:
	ldr	sp, =STACK_END_ADDR
	stmfd	sp!, {r0-r3}
	/* TODO */
	ldmfd	sp!, {r0-r3}
	ldr	lr, =_cpu_idle
	ldr	pc, =start_kernel

.ltorg

