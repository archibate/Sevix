/*
 * arch.c
 *
 * - 平台相关的代码
 */

#include <kernel.h>
#include <mach/arch.h>
#include <asm/drives.h>
#include <init.h>

void __setup_arch(void *boot_args)
{
	__serial_init();
	serial_puts("Hello, ARM World!\n\r");
}

