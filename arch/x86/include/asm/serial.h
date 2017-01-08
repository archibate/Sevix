#ifndef	_ASM_SERIAL_H_INCLUDED
#define	_ASM_SERIAL_H_INCLUDED

#ifdef	__CLANG__

extern void serial_puts(const char *str);
extern void serial_putc(char c);
extern int serial_printf(const char *fmt, ...);
extern void __serial_init();

#endif

#endif

