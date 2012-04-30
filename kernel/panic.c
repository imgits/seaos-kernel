/* panic.c: Copyright (c) 2010 Daniel Bittman
 * Functions for kernel crashes and exceptions */
#include <kernel.h>
#include <vargs.h>
#include <asm/system.h>
#include <task.h>
#include <elf.h>
extern int vsprintf(char *buf, const char *fmt, va_list args);
volatile int panicing=0;
extern unsigned end;

static inline void _set_lowercase(char *b)
{
	while(*b) {
		if(*b >= 'A' && *b <= 'Z')
			*b += 32;
		b++;
	}
}

void panic(char flags, char *fmt, ...)
{
	__super_cli();
	int second_panic = panicing++;
	int pid=0;
	task_t *t=current_task;
	if(t) pid=t->pid;
	set_current_task_dp(0);
	kprintf("\n\n*** kernel panic (%d) - ", second_panic+1);
	
	char buf[512];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	_set_lowercase(buf);
	kprintf(buf);
	
	kprintf(" ***\n");
	
	if(t) 
		kprintf("current_task=%x:%d, sys=%d, flags=%x, F=%x: ", t, t->pid, t->system, t->flags, t->flag);
		
	if(pid && !second_panic && !(flags & PANIC_NOSYNC))
	{
		kprintf("syncing...");
		sys_sync();
		kprintf("Done\n");
	} else
		kprintf("not syncing\n");
	__super_cli();
	for(;;)
	{
		asm("cli; hlt;");
	}
}

void panic_assert(const char *file, u32int line, const char *desc)
{
	__super_cli();
	task_critical();
	printk(KERN_CRIT, "Some code f*cked up (%s @ %d): %s.\n", file, line, desc);
	panic(0, "Assertion Fail");
}
