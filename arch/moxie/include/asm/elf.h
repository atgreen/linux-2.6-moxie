/*
 * Copyright (C) 2010 Anthony Green <green@moxielogic.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MOXIE_ELF_H
#define _ASM_MOXIE_ELF_H

/*
 * Note there is no "official" ELF designation for Moxie.
 * This one corresponds to binutils' src/include/elf/common.h.
 */
#define EM_MOXIE	0xFEED
#define ELF_ARCH	EM_MOXIE

/*
 * This is used to ensure we don't load something for the wrong architecture.
 */
#define elf_check_arch(x) ((x)->e_machine == EM_MOXIE)

/*
 * These are used to set parameters in the core dumps.
 */
#define ELF_CLASS	ELFCLASS32

#endif /* _ASM_MOXIE_ELF_H */
