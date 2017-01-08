#ifndef	_SC_H_INCLUDED
#define	_SC_H_INCLUDED

#include <mm.h>
#include <mach/context.h>

#define	MAX_PIDS	64
#define	STACK_PAGES	2
#define	STACK_SIZE	(STACK_SIZE * PAGE_SIZE)

#ifdef	__CLANG__

typedef unsigned int pid_t;
enum task_state {
	TASK_UNINIT	= 0,
	TASK_ACTIVE	= 1,
	TASK_WAITING	= 2,
	TASK_ONSEND	= 4,
	TASK_ONRECV	= 8,
	TASK_ONBOTH	= TASK_ONSEND | TASK_ONRECV,
	TASK_INVALID	= TASK_UNINIT
};

struct task {
	struct task *next;
	addr_t sp;
	enum task_state state;
	union task_stack *stack;
};

union task_stack {
	//struct task *ti;
	union page pages[STACK_PAGES];
};

extern void __sc_init();
extern int init_thread_main();
extern int kthreadd_thread_main();
extern void schedule();
extern pid_t kernel_thread(int (* entry) (void), enum task_state attr);
void __noreturn kernel_thread_exit(int status);
extern void __noreturn _cpu_halt();
extern void __noreturn _cpu_idle();
extern struct task *__fastcall switch_context(struct task *from, struct task *to);
extern struct task ttable[MAX_PIDS];
extern struct task *current, *current_prev;

#endif	/* __CLANG__ */

#endif

