/*
 * main.c
 *
 * - 内核初始化
 */

#include <kernel.h>
#include <init.h>
#include <sc.h>
#include <mm.h>
#include <uart.h>

void start_kernel(void)
{
	__uart_init();
	uart_puts("UART serial initialized\n\r");
	__mm_init();
	__sc_init();
	uart_puts("\n\rSevix version 0.1 (bate@archimint)\n\r"
		  /*"kernel build 00001-243 2016/12/14 02:38:46 UTC\n\r"*/
		  "gcc version 6.2.0 (Arch Repository)\n\r"
		  "configures: ARCH=arm PLAT=vexpress-a15 BITS=32 "
		  "ARMCPU=cortex-a15 ARMVER=armv7\n\r"
		  "hardware: ARM Versatile Express Cortex-A15\n\r"
		  "\nWelcome to Sevix   (C) PythonBin 2016\n\r\n");
	uart_puts("ksh version 0.1\n\r"
		  "type help to view the helping mannal\n\r");
	uart_puts("ksh> ");
#if 0
	char c;
	do {
		c = uart_getc();
		uart_putc(c);
	} while (c != '\n');
#endif
	return;		/* will return to _cpu_idle() */
}

