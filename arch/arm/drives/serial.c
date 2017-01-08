/*
 * serial.c
 *
 * - 串口 I/O 的驱动
 */

#include <kernel.h>
#include <asm/serial.h>
#include <asm/iomem.h>
#include <string.h>
#include <vsprintf.h>

void __serial_init()
{
	IORAOF(UART)[0x24/4] = 0x10;
	IORAOF(UART)[12] = 0xC3000001;
}

void serial_putc(char c)
{
	IORAOF(UART)[0] = c;
}

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

