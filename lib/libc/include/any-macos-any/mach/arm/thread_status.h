/*
 * Copyright (c) 2007 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * FILE_ID: thread_status.h
 */


#ifndef _ARM_THREAD_STATUS_H_
#define _ARM_THREAD_STATUS_H_

#if defined (__arm__) || defined (__arm64__)

#include <mach/machine/_structs.h>
#include <mach/machine/thread_state.h>
#include <mach/message.h>
#include <mach/vm_types.h>


/*
 *    Support for determining the state of a thread
 */


/*
 *  Flavors
 */

#define ARM_THREAD_STATE         1
#define ARM_UNIFIED_THREAD_STATE ARM_THREAD_STATE
#define ARM_VFP_STATE            2
#define ARM_EXCEPTION_STATE      3
#define ARM_DEBUG_STATE          4 /* pre-armv8 */
#define THREAD_STATE_NONE        5
#define ARM_THREAD_STATE64       6
#define ARM_EXCEPTION_STATE64    7
//      ARM_THREAD_STATE_LAST    8 /* legacy */
#define ARM_THREAD_STATE32       9
#define ARM_EXCEPTION_STATE64_V2 10


/* API */
#define ARM_DEBUG_STATE32        14
#define ARM_DEBUG_STATE64        15
#define ARM_NEON_STATE           16
#define ARM_NEON_STATE64         17
#define ARM_CPMU_STATE64         18


#define ARM_PAGEIN_STATE         27

/* API */
#define ARM_SME_STATE            28
#define ARM_SVE_Z_STATE1         29
#define ARM_SVE_Z_STATE2         30
#define ARM_SVE_P_STATE          31
#define ARM_SME_ZA_STATE1        32
#define ARM_SME_ZA_STATE2        33
#define ARM_SME_ZA_STATE3        34
#define ARM_SME_ZA_STATE4        35
#define ARM_SME_ZA_STATE5        36
#define ARM_SME_ZA_STATE6        37
#define ARM_SME_ZA_STATE7        38
#define ARM_SME_ZA_STATE8        39
#define ARM_SME_ZA_STATE9        40
#define ARM_SME_ZA_STATE10       41
#define ARM_SME_ZA_STATE11       42
#define ARM_SME_ZA_STATE12       42
#define ARM_SME_ZA_STATE13       44
#define ARM_SME_ZA_STATE14       45
#define ARM_SME_ZA_STATE15       46
#define ARM_SME_ZA_STATE16       47
#define ARM_SME2_STATE           48

#define THREAD_STATE_FLAVORS     50     /* This must be updated to 1 more than the highest numerical state flavor */

#ifndef ARM_STATE_FLAVOR_IS_OTHER_VALID
#define ARM_STATE_FLAVOR_IS_OTHER_VALID(_flavor_) 0
#endif

#define FLAVOR_MODIFIES_CORE_CPU_REGISTERS(x) \
((x == ARM_THREAD_STATE) ||     \
 (x == ARM_THREAD_STATE32) ||   \
 (x == ARM_THREAD_STATE64))

#define VALID_THREAD_STATE_FLAVOR(x) \
	((x == ARM_THREAD_STATE) ||           \
	 (x == ARM_VFP_STATE) ||              \
	 (x == ARM_EXCEPTION_STATE) ||        \
	 (x == ARM_DEBUG_STATE) ||            \
	 (x == THREAD_STATE_NONE) ||          \
	 (x == ARM_THREAD_STATE32) ||         \
	 (x == ARM_THREAD_STATE64) ||         \
	 (x == ARM_EXCEPTION_STATE64) ||      \
	 (x == ARM_EXCEPTION_STATE64_V2) ||      \
	 (x == ARM_NEON_STATE) ||             \
	 (x == ARM_NEON_STATE64) ||           \
	 (x == ARM_DEBUG_STATE32) ||          \
	 (x == ARM_DEBUG_STATE64) ||          \
	 (x == ARM_PAGEIN_STATE) ||           \
	 (ARM_STATE_FLAVOR_IS_OTHER_VALID(x)))
/*
 * VALID_THREAD_STATE_FLAVOR() intentionally excludes ARM_SME_STATE through
 * ARM_SME2_STATE, since these are not currently supported inside Mach exception
 * ports.
 */

struct arm_state_hdr {
	uint32_t flavor;
	uint32_t count;
};
typedef struct arm_state_hdr arm_state_hdr_t;

typedef _STRUCT_ARM_THREAD_STATE   arm_thread_state_t;
typedef _STRUCT_ARM_THREAD_STATE   arm_thread_state32_t;
typedef _STRUCT_ARM_THREAD_STATE64 arm_thread_state64_t;

#if __DARWIN_C_LEVEL >= __DARWIN_C_FULL && defined(__arm64__)

/* Accessor macros for arm_thread_state64_t pointer fields */

/* Return pc field of arm_thread_state64_t as a data pointer value */
#define arm_thread_state64_get_pc(ts) \
	        __darwin_arm_thread_state64_get_pc(ts)
/* Return pc field of arm_thread_state64_t as a function pointer. May return
 * NULL if a valid function pointer cannot be constructed, the caller should
 * fall back to the arm_thread_state64_get_pc() macro in that case. */
#define arm_thread_state64_get_pc_fptr(ts) \
	        __darwin_arm_thread_state64_get_pc_fptr(ts)
/* Set pc field of arm_thread_state64_t to a function pointer */
#define arm_thread_state64_set_pc_fptr(ts, fptr) \
	        __darwin_arm_thread_state64_set_pc_fptr(ts, fptr)
/* Set pc field of arm_thread_state64_t to an already signed function pointer */
#define arm_thread_state64_set_pc_presigned_fptr(ts, fptr) \
	        __darwin_arm_thread_state64_set_pc_presigned_fptr(ts, fptr)
/* Return lr field of arm_thread_state64_t as a data pointer value */
#define arm_thread_state64_get_lr(ts) \
	        __darwin_arm_thread_state64_get_lr(ts)
/* Return lr field of arm_thread_state64_t as a function pointer. May return
 * NULL if a valid function pointer cannot be constructed, the caller should
 * fall back to the arm_thread_state64_get_lr() macro in that case. */
#define arm_thread_state64_get_lr_fptr(ts) \
	        __darwin_arm_thread_state64_get_lr_fptr(ts)
/* Set lr field of arm_thread_state64_t to a function pointer */
#define arm_thread_state64_set_lr_fptr(ts, fptr) \
	        __darwin_arm_thread_state64_set_lr_fptr(ts, fptr)
/* Set lr field of arm_thread_state64_t to an already signed function pointer */
#define arm_thread_state64_set_lr_presigned_fptr(ts, fptr) \
	        __darwin_arm_thread_state64_set_lr_presigned_fptr(ts, fptr)
/* Return sp field of arm_thread_state64_t as a data pointer value */
#define arm_thread_state64_get_sp(ts) \
	        __darwin_arm_thread_state64_get_sp(ts)
/* Set sp field of arm_thread_state64_t to a data pointer value */
#define arm_thread_state64_set_sp(ts, ptr) \
	        __darwin_arm_thread_state64_set_sp(ts, ptr)
/* Return fp field of arm_thread_state64_t as a data pointer value */
#define arm_thread_state64_get_fp(ts) \
	        __darwin_arm_thread_state64_get_fp(ts)
/* Set fp field of arm_thread_state64_t to a data pointer value */
#define arm_thread_state64_set_fp(ts, ptr) \
	        __darwin_arm_thread_state64_set_fp(ts, ptr)
/* Strip ptr auth bits from pc, lr, sp and fp field of arm_thread_state64_t */
#define arm_thread_state64_ptrauth_strip(ts) \
	        __darwin_arm_thread_state64_ptrauth_strip(ts)

#endif /* __DARWIN_C_LEVEL >= __DARWIN_C_FULL && defined(__arm64__) */

struct arm_unified_thread_state {
	arm_state_hdr_t ash;
	union {
		arm_thread_state32_t ts_32;
		arm_thread_state64_t ts_64;
	} uts;
};
#define ts_32 uts.ts_32
#define ts_64 uts.ts_64
typedef struct arm_unified_thread_state arm_unified_thread_state_t;

#define ARM_THREAD_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_thread_state_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE32_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_thread_state32_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE64_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_thread_state64_t)/sizeof(uint32_t)))
#define ARM_UNIFIED_THREAD_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_unified_thread_state_t)/sizeof(uint32_t)))


typedef _STRUCT_ARM_VFP_STATE         arm_vfp_state_t;
typedef _STRUCT_ARM_NEON_STATE        arm_neon_state_t;
typedef _STRUCT_ARM_NEON_STATE        arm_neon_state32_t;
typedef _STRUCT_ARM_NEON_STATE64      arm_neon_state64_t;


typedef _STRUCT_ARM_EXCEPTION_STATE   arm_exception_state_t;
typedef _STRUCT_ARM_EXCEPTION_STATE   arm_exception_state32_t;
typedef _STRUCT_ARM_EXCEPTION_STATE64 arm_exception_state64_t;
typedef _STRUCT_ARM_EXCEPTION_STATE64_V2 arm_exception_state64_v2_t;

typedef _STRUCT_ARM_DEBUG_STATE32     arm_debug_state32_t;
typedef _STRUCT_ARM_DEBUG_STATE64     arm_debug_state64_t;

typedef _STRUCT_ARM_PAGEIN_STATE      arm_pagein_state_t;

typedef _STRUCT_ARM_SME_STATE         arm_sme_state_t;
typedef _STRUCT_ARM_SVE_Z_STATE       arm_sve_z_state_t;
typedef _STRUCT_ARM_SVE_P_STATE       arm_sve_p_state_t;
typedef _STRUCT_ARM_SME_ZA_STATE      arm_sme_za_state_t;
typedef _STRUCT_ARM_SME2_STATE        arm_sme2_state_t;

/*
 * Otherwise not ARM64 kernel and we must preserve legacy ARM definitions of
 * arm_debug_state for binary compatability of userland consumers of this file.
 */
#if defined(__arm__)
typedef _STRUCT_ARM_DEBUG_STATE        arm_debug_state_t;
#elif defined(__arm64__)
typedef _STRUCT_ARM_LEGACY_DEBUG_STATE arm_debug_state_t;
#else /* defined(__arm__) */
#error Undefined architecture
#endif /* defined(__arm__) */

#define ARM_VFP_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_vfp_state_t)/sizeof(uint32_t)))

#define ARM_EXCEPTION_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_exception_state_t)/sizeof(uint32_t)))

#define ARM_EXCEPTION_STATE64_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_exception_state64_t)/sizeof(uint32_t)))

#define ARM_EXCEPTION_STATE64_V2_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_exception_state64_v2_t)/sizeof(uint32_t)))

#define ARM_DEBUG_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_debug_state_t)/sizeof(uint32_t)))

#define ARM_DEBUG_STATE32_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_debug_state32_t)/sizeof(uint32_t)))

#define ARM_PAGEIN_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_pagein_state_t)/sizeof(uint32_t)))

#define ARM_DEBUG_STATE64_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_debug_state64_t)/sizeof(uint32_t)))

#define ARM_NEON_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_neon_state_t)/sizeof(uint32_t)))

#define ARM_NEON_STATE64_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_neon_state64_t)/sizeof(uint32_t)))

#define ARM_SME_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_sme_state_t)/sizeof(uint32_t)))

#define ARM_SVE_Z_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_sve_z_state_t)/sizeof(uint32_t)))

#define ARM_SVE_P_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_sve_p_state_t)/sizeof(uint32_t)))

#define ARM_SME_ZA_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_sme_za_state_t)/sizeof(uint32_t)))

#define ARM_SME2_STATE_COUNT ((mach_msg_type_number_t) \
	(sizeof (arm_sme2_state_t)/sizeof(uint32_t)))

#define MACHINE_THREAD_STATE       ARM_THREAD_STATE
#define MACHINE_THREAD_STATE_COUNT ARM_UNIFIED_THREAD_STATE_COUNT


/*
 * Largest state on this machine:
 */
#define THREAD_MACHINE_STATE_MAX THREAD_STATE_MAX


#endif /* defined (__arm__) || defined (__arm64__) */

#endif /* _ARM_THREAD_STATUS_H_ */
