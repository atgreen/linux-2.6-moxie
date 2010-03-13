/*
 * Copyright (C) 2010 Anthony Green <green@moxielogic.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

/* A pointer to the exception handler routine.  */
#define MOXIE_SREG_EXCEPTION_HANDLER  1

/* The exception type.  */
#define MOXIE_SREG_EXCEPTION_TYPE     2

/* The software interrupt number.  */
#define MOXIE_SREG_SWI_NUMBER         3

/* The Linux kernel stack pointer.  */
#define MOXIE_SREG_KSP                4

/* A scratch register.  */
#define MOXIE_SREG_SCRATCH            5

/* A pointer to the Device Tree Blob at startup.  */
#define MOXIE_SREG_DTB                9


