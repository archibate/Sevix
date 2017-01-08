/*
 * mm.c
 *
 * - 内核的多任务调度
 */

#include <kernel.h>
#include <sc.h>
#include <mm.h>
#include <init.h>
#include <string.h>
#include <asm/serial.h>

static pid_t find_free_task_pid();

struct task ttable[MAX_PIDS] = {0};
struct task *current = NULL, *current_prev = NULL;
static int (* kthreadd_create_list[10]) (void) = {NULL};
static size_t kthreadd_create_list_index = 0;

static int test_thread_main();

void __sc_init()
{
	current = &ttable[0];
	memset(current, 0, sizeof(*current));
	current->next = current;
	current->state = TASK_ACTIVE;
	current_prev = current;
	kernel_thread(init_thread_main, TASK_ACTIVE);
	return;
	kernel_thread(kthreadd_thread_main, TASK_ACTIVE);
}

static int test_thread_main()
{
	serial_puts("Hello, World!\n\r");
	return 0;
}

int kthreadd_thread_main()
{
	serial_puts("hello, this is kthreadd\n\r");
	return 0;
	LOOP {
		if (likely(!kthreadd_create_list[kthreadd_create_list_index])) {
			schedule();
			continue;
		}
		kernel_thread(kthreadd_create_list[kthreadd_create_list_index++], TASK_ACTIVE);
	}
	return 0;
}

int init_thread_main()
{
	serial_puts("hello, this is init\n\r");
	LOOP schedule();
	return 0;
}

void schedule()
{
	assert(current);
	current_prev = current;
	current = current->next;
	switch_context(current_prev, current);
}

void __noreturn kernel_thread_exit(int status)
{
	current_prev->next = current->next;
	kufree(current->stack);
	schedule();
	assert(0);
	_cpu_halt();
}

pid_t kernel_thread(int (* entry) (void), enum task_state attr)
{
	assert(entry);
	pid_t pid = find_free_task_pid();
	assert(pid);
	struct task *new = &ttable[pid];
	assert(new);
	memset(new, 0, sizeof(struct task));
	new->stack = kualloc(STACK_PAGES);
	assert(new->stack);
	struct stack_context *context = (struct stack_context *)
		((addr_t) (new->stack) + sizeof(*new->stack) - sizeof(*context));
	memset(context, 0, sizeof(*context));
	extern void __noreturn __kernel_thread_starter(void);
	context->ret_addr = (addr_t) __kernel_thread_starter;
#ifdef	_ARCH_arm
#define	FIRST_CONTEXT_SAVED_REG	arm_r4
#endif
#ifdef	_ARCH_x86_32
#define	FIRST_CONTEXT_SAVED_REG	x86_32_ebx
#endif
#ifdef	_ARCH_x86_64
#define	FIRST_CONTEXT_SAVED_REG	x86_64_r10
#endif
	context->FIRST_CONTEXT_SAVED_REG = (r_t) entry;
	new->sp = (addr_t) context;
	new->state = attr;
	new->next = current->next;
	current->next = new;
	if (attr & TASK_ACTIVE) {
		schedule();
	}
	return pid;
}

pid_t find_free_task_pid()
{
	for (size_t pid = 1; pid < MAX_PIDS; ++pid)
		if (ttable[pid].state == TASK_UNINIT)
			return (pid_t) pid;
	return 0;
}

