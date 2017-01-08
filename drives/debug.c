/*
 * debug.c
 *
 * - 内核调试
 */

#include <kernel.h>
#include <asm/serial.h>
#include <string.h>
#include <vsprintf.h>
#include <sc.h>

void __debug_tool __debug_print(const char *s)
{
	serial_puts(s);
}

int __debug_tool __debug_printf(const char *fmt, ...)
{
	static char buf[1024];
	va_list va;
	va_start(va, fmt);
	int res = vsprintf_s(buf, sizeof(buf), fmt, va);
	va_end(va);
	__debug_print(buf);
	return res;
}

void __debug_tool __debug_assertion_panic(const char *expr, const char *file, unsigned int line, const char *info) {
	//__debug_print("PANIC: Assertion failed in " #file ":" #line ":(" #x "):" info);
	__debug_printf("PANIC: In %s:%u: assert(%s) failed: %s\n\r", file, line, expr, info);
	_cpu_halt();
}

