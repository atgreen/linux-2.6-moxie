/*
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MOXIE_HARDIRQ_H
#define _ASM_MOXIE_HARDIRQ_H

/* should be defined in each interrupt controller driver */
extern unsigned int get_irq(struct pt_regs *regs);

#define ack_bad_irq ack_bad_irq
void ack_bad_irq(unsigned int irq);
#include <asm-generic/hardirq.h>

#endif /* _ASM_MOXIE_HARDIRQ_H */
