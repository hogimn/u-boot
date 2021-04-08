/*
 * This was automagically generated from arch/arm/tools/mach-types!
 * Do NOT edit
 */

#ifndef __ASM_ARM_MACH_TYPE_H
#define __ASM_ARM_MACH_TYPE_H

#ifndef __ASSEMBLY__
/* The type of machine we're running on */
extern unsigned int __machine_arch_type;
#endif

/* see arch/arm/kernel/arch.c for a description of these */
#define MACH_TYPE_SMDK2416             1685
#define MACH_TYPE_SMDS2450             3495


/*
 * These have not yet been registered
 */

#ifndef machine_arch_type
#define machine_arch_type	__machine_arch_type
#endif

#endif
