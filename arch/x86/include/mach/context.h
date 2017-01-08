#ifndef	_MACH_CONTEXT_H_INCLUDED
#define	_MACH_CONTEXT_H_INCLUDED

#ifdef	__CLANG__

struct stack_context {
	r_t x86_32_ebx, x86_32_edi, x86_32_esi, x86_32_ebp;
	addr_t ret_addr /* x86_32_eip */;
} __packed;

#endif	/* __CLANG__ */

#endif


