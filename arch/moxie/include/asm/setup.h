/*
 * Copyright (C) 2009 Anthony Green <green@moxielogic.com>
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MOXIE_SETUP_H
#define _ASM_MOXIE_SETUP_H

#define COMMAND_LINE_SIZE	256

# ifndef __ASSEMBLY__

#  ifdef __KERNEL__
extern unsigned int boot_cpuid; /* move to smp.h */

extern char cmd_line[COMMAND_LINE_SIZE];

void early_printk(const char *fmt, ...);

int setup_early_printk(char *opt);
void disable_early_printk(void);

void heartbeat(void);
void setup_heartbeat(void);

unsigned long long sched_clock(void);

#   ifdef CONFIG_MMU
extern void mmu_reset(void);
extern void early_console_reg_tlb_alloc(unsigned int addr);
#   endif /* CONFIG_MMU */

void time_init(void);
void init_IRQ(void);
void machine_early_init(const char *cmdline);

void machine_restart(char *cmd);
void machine_shutdown(void);
void machine_halt(void);
void machine_power_off(void);

#  endif/* __KERNEL__ */
# endif /* __ASSEMBLY__ */
#endif /* _ASM_MOXIE_SETUP_H */
