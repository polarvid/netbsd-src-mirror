/*	$NetBSD: mcontext.h,v 1.15 2024/11/04 15:45:24 christos Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Copyright (c) 1999, 2003 Marcel Moolenaar
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _IA64_MCONTEXT_H_
#define _IA64_MCONTEXT_H_

#include <machine/_regset.h>

/* XXX fix this, just get to compile for now */
#define _NGREG	1

#ifndef __ASSEMBLER__
typedef unsigned long __greg_t;
typedef __greg_t __gregset_t[_NGREG];

typedef struct {
  union _ia64_fpreg __fpregs[_NGREG];
} __fpregset_t;

#endif /* __ASSEMBLER__ */

/*
 * The mc_flags field provides the necessary clues when dealing with the gory
 * details of ia64 specific contexts. A comprehensive explanation is added for
 * everybody's sanity, including the author's.
 *
 * The first and foremost variation in the context is synchronous contexts
 * (= synctx) versus asynchronous contexts (= asynctx). A synctx is created
 * synchronously WRT program execution and has the advantage that none of the
 * scratch registers have to be saved. They are assumed to be clobbered by the
 * call to the function that creates the context. An asynctx needs to have the
 * scratch registers preserved because it can describe any point in a thread's
 * (or process') execution.
 * The second variation is for synchronous contexts. When the kernel creates
 * a synchronous context if needs to preserve the scratch registers, because
 * the syscall argument and return values are stored there in the trapframe
 * and they need to be preserved in order to restart a syscall or return the
 * proper return values. Also, the IIP and CFM fields need to be preserved
 * as they point to the syscall stub, which the kernel saves as a favor to
 * userland (it keeps the stubs small and simple).
 *
 * Below a description of the flags and their meaning:
 *
 *	_MC_FLAGS_ASYNC_CONTEXT
 *		If set, indicates that mc_scratch and mc_scratch_fp are both
 *		valid. IFF not set, _MC_FLAGS_SYSCALL_CONTEXT indicates if the
 *		synchronous context is one corresponding to a syscall or not.
 *		Only the kernel is expected to create such a context and it's
 *		probably wise to let the kernel restore it.
 *	_MC_FLAGS_HIGHFP_VALID
 *		If set, indicates that the high FP registers (f32-f127) are
 *		valid. This flag is very likely not going to be set for any
 *		sensible synctx, but is not explicitly disallowed. Any synctx
 *		that has this flag may or may not have the high FP registers
 *		restored. In short: don't do it.
 *	_MC_FLAGS_SYSCALL_CONTEXT
 *		If set (hence _MC_FLAGS_ASYNC_CONTEXT is not set) indicates
 *		that the scratch registers contain syscall arguments and
 *		return values and that additionally IIP and CFM are valid.
 *		Only the kernel is expected to create such a context. It's
 *		probably wise to let the kernel restore it.
 */

typedef struct __mcontext {
	unsigned long		mc_flags;
#define	_MC_FLAGS_ASYNC_CONTEXT		0x0001
#define	_MC_FLAGS_HIGHFP_VALID		0x0002
#define	_MC_FLAGS_SYSCALL_CONTEXT	0x0008
	unsigned long		_reserved_;
	struct _special		mc_special;
	struct _callee_saved	mc_preserved;
	struct _callee_saved_fp	mc_preserved_fp;
	struct _caller_saved	mc_scratch;
	struct _caller_saved_fp	mc_scratch_fp;
	struct _high_fp		mc_high_fp;

	/* XXX fix */
	__gregset_t		__gregs;
	__fpregset_t		__fpregs;
} mcontext_t;

#define _UC_MACHINE_SP(uc)	((uc)->uc_mcontext.mc_special.sp)  /* gregs[12] */
#define _UC_MACHINE_FP(uc)	0 /* Not supported in target */
#define	_UC_MACHINE_PC(uc)	((uc)->uc_mcontext.mc_special.iip)
#define	_UC_MACHINE_INTRV(uc)	((uc)->uc_mcontext.mc_scratch.gr8) /* gregs[8] */
#define _UC_MACHINE_SET_PC(uc, pc)	(uc)->uc_mcontext.mc_special.iip = (pc)

#define	_UC_TLSBASE	_UC_MD_BIT16
#define	_UC_SETSTACK	_UC_MD_BIT17
#define	_UC_CLRSTACK	_UC_MD_BIT18


#if defined(_RTLD_SOURCE) || defined(_LIBC_SOURCE) || \
    defined(__LIBPTHREAD_SOURCE__)
#include <sys/tls.h>

__BEGIN_DECLS
static __inline void *
__lwp_getprivate_fast(void)
{
	return (void*)0;
}
__END_DECLS

#endif

#endif	/* !_IA64_MCONTEXT_H_ */
