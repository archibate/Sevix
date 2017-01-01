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
#include <uart.h>

static pid_t find_free_task_pid();

struct task *ttable[MAX_PIDS] = {NULL};
struct task *current = NULL, *current_prev = NULL;
static union page init_thread_stack[STACK_PAGES];

void __sc_init()
{
	current = (struct task *) alloc_phys_tiny_page();
	memset(current, 0, sizeof(*current));
	current->next = current;
	current->state = TASK_ACTIVE;
	current_prev = current;
	ttable[0] = current;
	kernel_thread(init_thread_main, TASK_ACTIVE);
}

int init_thread_main()
{
	uart_puts("hello, this is init\n\r");
	schedule();
	_cpu_halt();
}

void schedule()
{
	assert(current);
	current_prev = current;
	current = current->next;
	switch_context(current_prev, current);
}

pid_t kernel_thread(int (* entry) (void), enum task_state attr)
{
	assert(entry);
	struct task *new = (struct task *) alloc_phys_tiny_page();
	assert(new);
	memset(new, 0, sizeof(*new));
	union page *stack = init_thread_stack;
	assert(stack);
	struct stack_context *context = ((struct stack_context *) (stack + STACK_PAGES)) - 1;
	context->ret_addr = (addr_t) entry;
	new->sp = (addr_t) context;
	new->next = current->next;
	current->next = new;
	pid_t pid = find_free_task_pid();
	assert(pid);
	ttable[pid] = new;
	if (likely(attr & TASK_ACTIVE))
		schedule();
	return pid;
}

pid_t find_free_task_pid()
{
	for (size_t pid = 1; pid < MAX_PIDS; ++pid)
		if (!ttable[pid])
			return (pid_t) pid;
	return 0;
}

