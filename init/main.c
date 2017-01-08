/*
 * main.c
 *
 * - 内核初始化
 */

#include <kernel.h>
#include <init.h>
#include <sc.h>
#include <mm.h>
#include <mach/arch.h>
#include <asm/serial.h>

void start_kernel(void)
{
	__setup_arch(NULL);
	__mm_init();
	__sc_init();
	return;
	serial_printf("start_kernel: done\n\r");
	return;		/* will return to _cpu_idle() */
}

