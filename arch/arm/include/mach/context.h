#ifndef	_MACH_CONTEXT_H_INCLUDED
#define	_MACH_CONTEXT_H_INCLUDED

#ifdef	__CLANG__

struct stack_context {
	r_t arm_r4, arm_r5, arm_r6, arm_r7, arm_r8, arm_r9, arm_r10, arm_r11, arm_r12;
	addr_t ret_addr /* arm_pc */;
} __packed;

#endif	/* __CLANG__ */

#endif

