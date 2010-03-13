/*
 * Copyright (C) 2009, 2010 Anthony Green <green@moxielogic.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/page.h>
#include <asm/moxie_regs.h>

#ifdef CONFIG_MMU
#include <asm/setup.h> /* COMMAND_LINE_SIZE */
#include <asm/mmu.h>
#include <asm/processor.h>

.data
.global empty_zero_page
.align 12
empty_zero_page:
	.space	4096
.global swapper_pg_dir
swapper_pg_dir:
	.space	4096

#endif /* CONFIG_MMU */

	.text
ENTRY(_start)

	/* Initialize stack and frame pointers.  */
	ldi.l	$sp, init_thread_union + THREAD_SIZE - 4
	mov	$fp, $sp

	/* Initialize $r13 with init_task.  */
	ldi.l	$r13, init_task
	
	jsra	machine_early_init
	jmpa	start_kernel

	.text
ENTRY(ret_from_fork)
	jsra schedule_tail
	pop	$sp, $fp	/* Pop frame pointer.  */
	inc	$sp, 4		/* Skip $sp */
	pop	$sp, $r0
	pop	$sp, $r1
	pop	$sp, $r2
	pop	$sp, $r3
	pop	$sp, $r4
	pop	$sp, $r5
	pop	$sp, $r6
	pop	$sp, $r7
	pop	$sp, $r8
	pop	$sp, $r9
	pop	$sp, $r10
	pop	$sp, $r11
	pop	$sp, $r12
	pop	$sp, $r13
	pop	$sp, $r5	/* pc */
	inc	$sp, 4		/* Skip pt_mode.  */
	jmp	$r5

ENTRY(sys_clone)
	bad
ENTRY(sys_rt_sigreturn_wrapper)
	bad
ENTRY(sys_rt_sigsuspend_wrapper)
	bad

ENTRY(sys_vfork)
	/* Move pt_regs to the first argument register and jump to
	   moxie_vfork.  */
	mov	$r0, $r8	
	jmpa	moxie_vfork

ENTRY(__umodsi3)
	bad
ENTRY(__udivsi3)
	bad
ENTRY(__mulsi3)
	bad
ENTRY(__modsi3)
	bad
ENTRY(__divsi3)
	bad

# struct task_struct *__switch_to(struct thread_info *prev,
#				  struct thread_info *next,
#				  struct task_struct *prev_task)
	
ENTRY(_switch_to)
	sto.l	TI_CPU_CONTEXT+CC_FP($r0), $fp
	sto.l	TI_CPU_CONTEXT+CC_SP($r0), $sp
	sto.l	TI_CPU_CONTEXT+CC_R0($r0), $r0
	sto.l	TI_CPU_CONTEXT+CC_R1($r0), $r1
	sto.l	TI_CPU_CONTEXT+CC_R2($r0), $r2
	sto.l	TI_CPU_CONTEXT+CC_R3($r0), $r3
	sto.l	TI_CPU_CONTEXT+CC_R4($r0), $r4
	sto.l	TI_CPU_CONTEXT+CC_R5($r0), $r5
	sto.l	TI_CPU_CONTEXT+CC_R6($r0), $r6
	sto.l	TI_CPU_CONTEXT+CC_R7($r0), $r7
	sto.l	TI_CPU_CONTEXT+CC_R8($r0), $r8
	sto.l	TI_CPU_CONTEXT+CC_R9($r0), $r9
	sto.l	TI_CPU_CONTEXT+CC_R10($r0), $r10
	sto.l	TI_CPU_CONTEXT+CC_R11($r0), $r11
	sto.l	TI_CPU_CONTEXT+CC_R12($r0), $r12
	sto.l	TI_CPU_CONTEXT+CC_R13($r0), $r13

	ldo.l	$fp, TI_CPU_CONTEXT+CC_FP($r1)

	jsra	moxie_debug_switch_to
	mov	$r1, $r0
	
	ldo.l	$sp, TI_CPU_CONTEXT+CC_SP($r1)
	/* Return prev */
	mov	$r0, $r2
	ldo.l	$r3, TI_CPU_CONTEXT+CC_R3($r1)
	ldo.l	$r4, TI_CPU_CONTEXT+CC_R4($r1)
	ldo.l	$r5, TI_CPU_CONTEXT+CC_R5($r1)
	ldo.l	$r6, TI_CPU_CONTEXT+CC_R6($r1)
	ldo.l	$r7, TI_CPU_CONTEXT+CC_R7($r1)
	ldo.l	$r8, TI_CPU_CONTEXT+CC_R8($r1)
	ldo.l	$r9, TI_CPU_CONTEXT+CC_R9($r1)
	ldo.l	$r10, TI_CPU_CONTEXT+CC_R10($r1)
	ldo.l	$r11, TI_CPU_CONTEXT+CC_R11($r1)
	ldo.l	$r12, TI_CPU_CONTEXT+CC_R12($r1)
	ldo.l	$r13, TI_CPU_CONTEXT+CC_KSP($r1)
	ssr	$r13, MOXIE_SREG_KSP
	ldo.l	$r13, TI_CPU_CONTEXT+CC_R13($r1)
	ldo.l	$r1, TI_CPU_CONTEXT+CC_R1($r1)
	ret
