/*
 * uart.c
 *
 * - UART 串口
 */

#include <kernel.h>
#include <uart.h>
#include <string.h>
#include <init.h>

void uart_puts(const char *str)
{
	char c;
	while (c = *str++)
		uart_putc(c);
}

#ifdef	_DEBUG
void __debug_print(const char *s)
{
	uart_puts(s);
}

void __debug_assertion_panic(const char *expr, const char *file, unsigned int line, const char *info) {
	//__debug_print("PANIC: Assertion failed in " #file ":" #line ":(" #x "):" info);
	_cpu_halt();
}
#endif

