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

#define	BOOT_PGDIR_PADDR	0x0

#define	X86_IDT_ADDR	(KBASE + 0x80000)
#define	X86_GDT_ADDR	(KBASE + 0x80800)
#define	X86_TSS0_ADDR	(KBASE + 0x80A00)

.sect	.boot.text
.globl	__kernel_entry

.equ	MBOOT_HEADER_MAGIC,	0x1BADB002
.equ	MBOOT_HEADER_FLAGS,	3	# ..11: PAGE_ALIGN | MEM_INFO
.equ	MBOOT_CHECKSUM,		-(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

__multiboot_header:
	.long	MBOOT_HEADER_MAGIC
	.long	MBOOT_HEADER_FLAGS
	.long	MBOOT_CHECKSUM
__multiboot_ptr:
	.long	0

.equ	gdt_la,		X86_TSS0_ADDR
.equ	idt_la,		X86_TSS0_ADDR
.equ	tss0_la,	X86_TSS0_ADDR
.equ	gdt_pa,		KLIN2PHY(gdt_la)
.equ	idt_pa,		KLIN2PHY(idt_la)
.equ	tss0_pa,	KLIN2PHY(tss0_la)
.equ	boot_pd,	BOOT_PGDIR_PADDR
.equ	boot_ptes,	boot_pd + PGDIR_SIZE

__kernel_entry:
	cli
	cmpl	$0x10000000 + MBOOT_HEADER_MAGIC, %eax
	jne	not_multiboot
	movl	%ebx, __multiboot_ptr
not_multiboot:
	movl	$__boot_stack_top, %esp

	movl	$gdt_dat, %esi
	movl	$gdt_pa, %edi
	movl	$gdt_len / 4, %ecx
	cld
	rep
	movsl
	lgdt	gdtr

	movb	$0x11, %al
	outb	$0x20
	outb	$0xA0
	movb	$0x20, %al
	outb	$0x21
	movb	$0x28, %al
	outb	$0xA1
	movb	$0x04, %al
	outb	$0x21
	movb	$0x02, %al
	outb	$0xA1
	movb	$0x01, %al
	outb	$0x21
	outb	$0xA1
	movb	$0x00, %al
	outb	$0x21
	outb	$0xA1
	movl	$200, %ecx
	call	set_timer_freq

	lidt	idtr	/* idtr.limit = 0; idtr.base = 0: ignore any intr */

	call	setup_boot_pagging

	movl	$0x10, %eax
	movl	%eax, %ds
	movl	%eax, %es
	movl	%eax, %fs
	movl	%eax, %gs
	ljmp	$0x8, $vmem_entry

halt:	hlt
	jmp	halt

setup_boot_pagging:
	leal	boot_pd, %ebx
	movl	%ebx, %edi
	movl	$PGTAB_ENTS, %ecx
	xorl	%eax, %eax
	cld
	rep
	movsl
	movl	$boot_ptes | MMU_PAGE | MMU_MANAGER, (%ebx)
#if	KBASE & ~(~0 << PDE_SHIFT)
#error
#endif
#if	KBASE_OFFSET & ~(~0 << PDE_SHIFT)
#error
#endif
	leal	(KBASE >> PDE_SHIFT) * 4(%ebx), %edi
	leal	(KBASE_END >> PDE_SHIFT) * 4(%ebx), %edx
	movl	$boot_ptes | MMU_PGTAB | MMU_MANAGER, %eax
1:	stosl
	addl	$PGTAB_SIZE, %eax
	cmpl	%edx, %edi
	jb	1b
	leal	(KLIN2PHY(KBASE) >> PDE_SHIFT) * 4(%ebx), %edi
	leal	(KLIN2PHY(KBASE_END) >> PDE_SHIFT) * 4(%ebx), %edx
	movl	$boot_ptes | MMU_PGTAB | MMU_MANAGER, %eax
1:	stosl
	addl	$PGTAB_SIZE, %eax
	cmpl	%edx, %edi
	jb	1b
	leal	boot_ptes, %edi
	movl	$KLIN2PHY(KBASE) | MMU_PAGE | MMU_MANAGER, %eax
	movl	$KLIN2PHY(KBASE_END) | MMU_PAGE | MMU_MANAGER, %edx
1:	stosl
	addl	$PAGE_SIZE, %eax
	cmpl	%edx, %eax
	jb	1b
	movl	%ebx, %cr3
	movl	%cr0, %eax
	bts	$31, %eax
	movl	%eax, %cr0
	ret	/* yeah, this procture is so ugly. anyway, it just works */

set_timer_freq:
# AX = 1193180 / timer_freq
	movb	$0x43, %al
	outb	$0x36
	movl	$1193180, %eax
	divl	%ecx
	outb	$0x40
	movb	%ah, %al
	outb	$0x40
	ret

.sect	.boot.data
	.align	8
gdt_dat:
	.quad	0x0000000000000000	# NULL
	.quad	0x00CF9A000000FFFF	# SYS_CODE
	.quad	0x00CF92000000FFFF	# SYS_DATA
	.quad	0x00CFFA000000FFFF	# USR_CODE
	.quad	0x00CFF2000000FFFF	# USR_DATA
	#.quad	0xC000890838000067	# TSS0
	.long	((tss0_la & 0xFFFF) << 16) | 0x67
	.long	((tss0_la & 0xFF0000) >> 16) | (tss0_la & 0xFF000000) | 0x8900
	/*.quad	((tss0_la & 0xFFFFFF) << 16) | \
	((tss0_la & 0xFF000000) << 32+24-20) | \
	0x0000890000000067*/
.equ	gdt_len, . - gdt_dat
idtr:	.word	0	# ignored
	.long	0
gdtr:	.word	gdt_len - 1
	.long	gdt_la

.sect	.boot.bss
	.align	4
	.space	1024, 0xCC
__boot_stack_top:

.sect	.text

vmem_entry:
	pushl	$_cpu_halt
	movl	$start_kernel, %edx
	jmp	*%edx
