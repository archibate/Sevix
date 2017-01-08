/*
 * serial.c
 *
 * - 串口 I/O 的驱动
 */

#include <kernel.h>
#include <asm/serial.h>
#include <string.h>
#include <vsprintf.h>
#include <mm.h>

#define	VRAM_PADDR	0xB8000
#define	VRAM_VADDR	KPHY2LIN(VRAM_PADDR)
#define	VRAM		((short *) VRAM_VADDR)

static short *serial_chp = VRAM;

void __serial_init()
{
}

void serial_putc(char c)
{
	*serial_chp++ = c | 0x0700;
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

