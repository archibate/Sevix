#ifndef	_INIT_H_INCLUDED
#define	_INIT_H_INCLUDED

#ifdef	__CLANG__

extern void start_kernel(void);
extern void __noreturn _cpu_halt();
extern void __noreturn _cpu_idle();

#endif	/* __CLANG__ */

#endif

