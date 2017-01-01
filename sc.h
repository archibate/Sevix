#ifndef	_SC_H_INCLUDED
#define	_SC_H_INCLUDED

#include <mm.h>

#define	MAX_PIDS	64
#define	STACK_PAGES	2
#define	STACK_SIZE	(STACK_SIZE * PAGE_SIZE)

#ifdef	__CLANG__

typedef unsigned int pid_t;
enum task_state {
	TASK_UNINIT	= 0,
	TASK_ACTIVE	= 1,
	TASK_WAITING	= 2,
	TASK_UNKNOWN	= 3
};

struct task {
	struct task *next;
	addr_t sp;
	enum task_state state;
};

union task_stack {
	struct task *ti;
	union page stack[STACK_PAGES];
};

struct stack_context {
	r_t arm_r4, arm_r5, arm_r6, arm_r7, arm_r8, arm_r9, arm_r10, arm_r11, arm_r12;
	addr_t ret_addr;
};

#if	CONFIG_X86_32
struct stack_context {
	r_t x86_32_ebx, x86_32_edi, x86_32_esi, x86_32_ebp;
	addr_t ret_addr;
};
#endif

extern void __sc_init();
extern int init_thread_main();
extern void schedule();
extern pid_t kernel_thread(int (* entry) (void), enum task_state attr);
extern struct task *switch_context(struct task *from, struct task *to);
extern struct task *ttable[MAX_PIDS];
extern struct task *current, *current_prev;

#endif	/* __CLANG__ */

#endif

