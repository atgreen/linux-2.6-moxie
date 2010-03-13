/*
 * uClinux flat-format executables
 *
 * Copyright (C) 2010 Anthony Green <green@moxielogic.com>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
 */

#ifndef _ASM_MOXIE_FLAT_H
#define _ASM_MOXIE_FLAT_H

#include <asm/unaligned.h>

#define	flat_argvp_envp_on_stack()	1
#define	flat_old_ram_flag(flags)	(flags)
#define	flat_reloc_valid(reloc, size)	((reloc) <= (size))
#define	flat_set_persistent(relval, p)		0
#define	flat_get_addr_from_rp(rp, relval, flags, p)	get_unaligned(rp)
#define	flat_put_addr_at_rp(rp, val, relval)	put_unaligned(val,rp)
#define	flat_get_relocate_addr(rel)		(rel)

#endif /* _ASM_MOXIE_FLAT_H */
