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
#include <vsprintf.h>

void start_kernel(void)
{
	__uart_init();
	uart_puts("UART serial initialized\n\r");
	/*static char s[40];
	sprintf_s(s, sizeof(s), "hello, %s! this is my %dth OS! I've been wait for this moment for such a long time!!!\n\r", "ARM", 7);
	uart_puts(s);
	sprintf_s(s, sizeof(s), "p=%p\n\r", 0xf3c40);
	uart_puts(s);
	uart_puts("Kernel halting...");
	return;*/
	__mm_init();
	__sc_init();
	uart_puts("start_kernel: done\n\r");
	uart_puts("ksh> ");
	return;		/* will return to _cpu_idle() */
}

int kshell_thread_main()
{
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
	return 0;
}

