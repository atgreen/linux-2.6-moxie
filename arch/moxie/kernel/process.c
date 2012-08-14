/*
 * Copyright (C) 2009, 2010 Anthony Green <moxielogic.com>
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/bitops.h>
#include <asm/system.h>
#include <asm/pgalloc.h>

struct task_struct *moxie_debug_switch_to(struct thread_info *prev,
					struct thread_info *next,
					struct task_struct *prev_task)
{
	printk (KERN_NOTICE " >> SWITCHING TO 0x%x with KSP of 0x%x", 
		next, next->cpu_context.ksp);
	return next;
}

void show_regs(struct pt_regs *regs)
{
	printk(KERN_INFO " Registers dump: mode=%X\r\n", regs->pt_mode);
	printk(KERN_INFO " fp=%08lX, sp=%08lX, r0=%08lX, r1=%08lX\n",
	                        regs->fp, regs->sp, regs->r0, regs->r1);
	printk(KERN_INFO " r2=%08lX, r3=%08lX, r4=%08lX, r5=%08lX\n",
				regs->r2, regs->r3, regs->r4, regs->r5);
	printk(KERN_INFO " r6=%08lX, r7=%08lX, r8=%08lX, r9=%08lX\n",
				regs->r6, regs->r7, regs->r8, regs->r9);
	printk(KERN_INFO " r10=%08lX, r11=%08lX, r12=%08lX, r13=%08lX\n",
				regs->r10, regs->r11, regs->r12, regs->r13);
	printk(KERN_INFO " PC=%08lX\n", regs->pc);
}

void (*pm_idle)(void);
void (*pm_power_off)(void) = NULL;
EXPORT_SYMBOL(pm_power_off);

static int hlt_counter = 1;

void disable_hlt(void)
{
	hlt_counter++;
}
EXPORT_SYMBOL(disable_hlt);

void enable_hlt(void)
{
	hlt_counter--;
}
EXPORT_SYMBOL(enable_hlt);

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}
__setup("nohlt", nohlt_setup);

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}
__setup("hlt", hlt_setup);

void default_idle(void)
{
	if (!hlt_counter) {
		clear_thread_flag(TIF_POLLING_NRFLAG);
		smp_mb__after_clear_bit();
		local_irq_disable();
		while (!need_resched())
			cpu_sleep();
		local_irq_enable();
		set_thread_flag(TIF_POLLING_NRFLAG);
	} else
		while (!need_resched())
			cpu_relax();
}

void cpu_idle(void)
{
	set_thread_flag(TIF_POLLING_NRFLAG);

	/* endless idle loop with no priority at all */
	while (1) {
		void (*idle)(void) = pm_idle;

		if (!idle)
			idle = default_idle;

		tick_nohz_stop_sched_tick(1);
		while (!need_resched())
			idle();
		tick_nohz_restart_sched_tick();

		preempt_enable_no_resched();
		schedule();
		preempt_disable();
		check_pgt_cache();
	}
}

void flush_thread(void)
{
}

int copy_thread(unsigned long clone_flags, unsigned long usp,
		unsigned long unused,
		struct task_struct *p, struct pt_regs *regs)
{
	struct pt_regs *childregs = task_pt_regs(p);
	struct thread_info *ti = task_thread_info(p);
	unsigned int *frame = ((unsigned int *)childregs) - 3;

	printk(KERN_NOTICE "childregs       @ 0x%x\n", (unsigned int) childregs);
	printk(KERN_NOTICE "frame           @ 0x%x\n", (unsigned int) frame);
	printk(KERN_NOTICE "ti              @ 0x%x\n", (unsigned int) ti);
	printk(KERN_NOTICE "ti->cpu_context @ 0x%x\n", (unsigned int) &(ti->cpu_context));
	printk(KERN_NOTICE "task_stack_page @ 0x%x\n", (unsigned int) task_stack_page(p));
	printk(KERN_NOTICE "regs            @ 0x%x\n", (unsigned int) regs);
	printk(KERN_NOTICE "ksp             @ 0x%x\n", ((unsigned int) ti + THREAD_SIZE));

	*childregs = *regs;
	childregs->r0 = 0; /* child always returns 0 */
	childregs->r13 = (unsigned long) p; /* current */
	frame[1] = (unsigned long) ret_from_fork;

	if (user_mode(regs))
	{
		printk(KERN_NOTICE "*** USER STACK POINTER 1: 0x%x\n", (unsigned int) regs->sp);
		printk(KERN_NOTICE "*** USER STACK POINTER 2: 0x%x\n", (unsigned int) usp);
		childregs->sp = usp;
	}
	else
		childregs->sp = ((unsigned long) ti) + THREAD_SIZE;

	memset(&ti->cpu_context, 0, sizeof(struct cpu_context));
	ti->cpu_context.r0 = childregs->r0;
	ti->cpu_context.fp = (unsigned long) frame;
	ti->cpu_context.sp = (unsigned long) frame;
	ti->cpu_context.pc = (unsigned long) ret_from_fork;
	ti->cpu_context.r13 = (unsigned long) p; /* current */
	ti->cpu_context.ksp = ((unsigned long) ti) + THREAD_SIZE;

	if (clone_flags & CLONE_SETTLS)
		;

	return 0;
}

#ifndef CONFIG_MMU
/*
 * Return saved PC of a blocked thread.
 */
unsigned long thread_saved_pc(struct task_struct *tsk)
{
	struct cpu_context *ctx =
		&(((struct thread_info *)(tsk->stack))->cpu_context);

	return (unsigned long) ctx->pc;
}
#endif

static void kernel_thread_helper(int (*fn)(void *), void *arg)
{
	fn(arg);
	do_exit(-1);
}

int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));
	/* store them in non-volatile registers */
	regs.r0 = (unsigned long)fn;
	regs.r1 = (unsigned long)arg;
	regs.r13 = (unsigned long)current;
	regs.pc = (unsigned long)kernel_thread_helper;
	regs.pt_mode = 1;

	return do_fork(flags | CLONE_VM | CLONE_UNTRACED, 0,
			&regs, 0, NULL, NULL);
}
EXPORT_SYMBOL_GPL(kernel_thread);

unsigned long get_wchan(struct task_struct *p)
{
/* TBD (used by procfs) */
	return 0;
}

/* Set up a thread for executing a new program */
void start_thread(struct pt_regs *regs, unsigned long pc, unsigned long usp)
{
	set_fs(USER_DS);

	/* Push a fake stack frame, so that a "ret" instruction will
	   send us to the new thread.  */
	usp -= 12;
	((long *)usp)[1] = pc;     /* Return address.  */
	((long *)usp)[0] = usp + 12; /* Frame pointer.  */

	regs->sp = usp;
	regs->fp = usp;
	regs->pc = pc;

	regs->pt_mode = 0;
}

#ifdef CONFIG_MMU
#include <linux/elfcore.h>
/*
 * Set up a thread for executing a new program
 */
int dump_fpu(struct pt_regs *regs, elf_fpregset_t *fpregs)
{
	return 0; /* MicroBlaze has no separate FPU registers */
}
#endif /* CONFIG_MMU */
