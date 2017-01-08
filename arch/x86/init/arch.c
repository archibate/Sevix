/*
 * arch.c
 *
 * - 平台相关的代码
 */

#include <kernel.h>
#include <mach/arch.h>
#include <memap.h>
#include <asm/drives.h>

void __setup_arch(void *boot_args)
{
	__serial_init();
	serial_puts("Hello, X86 World!\n\r");
}

