/*
 * serial.c
 *
 * - 串口 I/O
 */

#include <kernel.h>
#include <serial.h>
#include <string.h>
#include <vsprintf.h>

void serial_puts(const char *str)
{
	char c;
	while (c = *str++)
		serial_putc(c);
}

int serial_printf(const char *fmt, ...)
{
	static char buf[1024];
	va_list va;
	va_start(va, fmt);
	int res = vsprintf_s(buf, sizeof(buf), fmt, va);
	va_end(va);
	serial_puts(buf);
	return res;
}

