#ifndef	_UART_H_INCLUDED
#define	_UART_H_INCLUDED

#ifdef	__CLANG__
extern void uart_puts(const char *str);
extern void uart_putc(char c);
extern void __uart_init();
#endif

#endif

