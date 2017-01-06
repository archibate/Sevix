/*
 * uart.c
 *
 * - UART 串口
 */

#include <kernel.h>
#include <uart.h>
#include <string.h>
#include <vsprintf.h>
#include <init.h>

void uart_puts(const char *str)
{
	char c;
	while (c = *str++)
		uart_putc(c);
}

int uart_printf(const char *fmt, ...)
{
	static char buf[1024];
	va_list va;
	va_start(va, fmt);
	int res = vsprintf_s(buf, sizeof(buf), fmt, va);
	va_end(va);
	uart_puts(buf);
	return res;
}

#ifdef	_DEBUG
void __debug_print(const char *s)
{
	uart_puts(s);
}

void __debug_assertion_panic(const char *expr, const char *file, unsigned int line, const char *info) {
	//__debug_print("PANIC: Assertion failed in " #file ":" #line ":(" #x "):" info);
	uart_printf("PANIC: In %s:%u: assert(%s) failed: %s\n\r", file, line, expr, info);
	_cpu_halt();
}
#endif

