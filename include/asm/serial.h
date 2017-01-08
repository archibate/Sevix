#ifndef	_ASM_SERIAL_H_INCLUDED
#define	_ASM_SERIAL_H_INCLUDED

#warning "including defult ASM header file: asm/serial.h"

#ifdef	__CLANG__

extern void serial_puts(const char *str);
extern void serial_putc(char c);

#endif

#endif

