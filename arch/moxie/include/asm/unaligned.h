/*
 * Copyright (C) 2010 Anthony Green <green@moxielogic.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MOXIE_UNALIGNED_H
#define _ASM_MOXIE_UNALIGNED_H

# ifdef __KERNEL__

# include <linux/unaligned/be_struct.h>
# include <linux/unaligned/le_byteshift.h>
# include <linux/unaligned/generic.h>

# define get_unaligned	__get_unaligned_be
# define put_unaligned	__put_unaligned_be

# endif	/* __KERNEL__ */
#endif /* _ASM_MOXIE_UNALIGNED_H */
