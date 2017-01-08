#ifndef	_SERIAL_H_INCLUDED
#define	_SERIAL_H_INCLUDED

#include <asm/serial.h>

#ifdef	__CLANG__

extern int serial_printf(const char *fmt, ...);
extern void __serial_init();

#endif

#endif

