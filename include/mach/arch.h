#ifndef	_MACH_ARCH_H_INCLUDED
#define	_MACH_ARCH_H_INCLUDED

#warning "including defult MACH header file: mach/arch.h"

#include <init.h>

#ifdef	__CLANG__

extern void __setup_arch(void *boot_args);

#endif	/* __CLANG__ */

#endif	/* _MACH_ARCH_H_INCLUDED */

