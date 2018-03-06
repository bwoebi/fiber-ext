/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Martin Schröder <m.schroeder2007@gmail.com>                 |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend.h"

#include "fiber.h"
#include "fiber_stack.h"

#if __linux && (__i386 || (__x86_64 && !__ILP32))
#define ZEND_FIBER_ASM 1
#elif __APPLE__ && (__i386 || (__x86_64 && !__ILP32))
#define ZEND_FIBER_ASM 1
#elif defined HAVE_UCONTEXT_H
#define ZEND_FIBER_UCONTEXT 1
#else
#define ZEND_FIBER_UNSUPPORTED 1
#endif

#ifdef ZEND_FIBER_ASM

#if __amd64
#define ZEND_FIBER_ASM_SAVED_REGISTERS 6
#elif __i386
#define ZEND_FIBER_ASM_SAVED_REGISTERS 4
#endif

#endif

#ifdef ZEND_FIBER_UCONTEXT
#include <ucontext.h>
#endif

typedef struct _zend_fiber_context_posix {
#ifdef ZEND_FIBER_ASM
	void **ctx;
#elif defined ZEND_FIBER_UCONTEXT
	ucontext_t ctx;
#endif

	zend_fiber_stack stack;
	zend_bool initialized;
	zend_bool root;
} zend_fiber_context_posix;

ZEND_API zend_fiber_context zend_fiber_create_root_context()
{
	zend_fiber_context_posix *context;

	context = emalloc(sizeof(zend_fiber_context_posix));
	ZEND_SECURE_ZERO(context, sizeof(zend_fiber_context_posix));

	context->initialized = 1;
	context->root = 1;

	return (zend_fiber_context) context;
}

ZEND_API zend_fiber_context zend_fiber_create_context()
{
	zend_fiber_context_posix *context;

	context = emalloc(sizeof(zend_fiber_context_posix));
	ZEND_SECURE_ZERO(context, sizeof(zend_fiber_context_posix));

	return (zend_fiber_context) context;
}

ZEND_API zend_bool zend_fiber_create(zend_fiber_context ctx, zend_fiber_func func, size_t stack_size)
{
	zend_fiber_context_posix *context;

	context = (zend_fiber_context_posix *) ctx;

	if (UNEXPECTED(context->initialized == 1)) {
		return 0;
	}

	if (!zend_fiber_stack_allocate(&context->stack, stack_size)) {
		return 0;
	}

#ifdef ZEND_FIBER_ASM

	context->ctx = (void **)(context->stack.size + (char *) context->stack.pointer);

	// Align stack and push fiber execution func.
	*--context->ctx = (void *) abort;
	*--context->ctx = (void *) func;

	context->ctx -= ZEND_FIBER_ASM_SAVED_REGISTERS;

	ZEND_SECURE_ZERO(context->ctx, sizeof(*context->ctx) * ZEND_FIBER_ASM_SAVED_REGISTERS);

#else

	if (getcontext(&context->ctx) == -1) {
		return 0;
	}

	context->ctx.uc_link = 0;
	context->ctx.uc_stack.ss_sp = context->stack.pointer;
	context->ctx.uc_stack.ss_size = context->stack.size;
	context->ctx.uc_stack.ss_flags = 0;

	makecontext(&context->ctx, func, 0);

#endif

	context->initialized = 1;

	return 1;
}

ZEND_API void zend_fiber_destroy(zend_fiber_context ctx)
{
	zend_fiber_context_posix *context;

	context = (zend_fiber_context_posix *) ctx;

	if (context != NULL) {
		if (!context->root && context->initialized) {
			zend_fiber_stack_free(&context->stack);
		}

		efree(context);
		context = NULL;
	}
}

#ifdef ZEND_FIBER_ASM

void __attribute__ ((__noinline__, __regparm__(2)))
zend_fiber_switch_context_asm(zend_fiber_context_posix *from, zend_fiber_context_posix *to) asm("zend_fiber_switch_context_asm");

asm (
	"\t.text\n"
	"\t.globl zend_fiber_switch_context_asm\n"
	"zend_fiber_switch_context_asm:\n"

#if __amd64
	"\tpushq %rbp\n"
	"\tpushq %rbx\n"
	"\tpushq %r12\n"
	"\tpushq %r13\n"
	"\tpushq %r14\n"
	"\tpushq %r15\n"
	"\tmovq %rsp, (%rdi)\n"
	"\tmovq (%rsi), %rsp\n"
	"\tpopq %r15\n"
	"\tpopq %r14\n"
	"\tpopq %r13\n"
	"\tpopq %r12\n"
	"\tpopq %rbx\n"
	"\tpopq %rbp\n"
	"\tpopq %rcx\n"
	"\tjmpq *%rcx\n"
#elif __i386
	"\tpushl %ebp\n"
	"\tpushl %ebx\n"
	"\tpushl %esi\n"
	"\tpushl %edi\n"
	"\tmovl %esp, (%eax)\n"
	"\tmovl (%edx), %esp\n"
	"\tpopl %edi\n"
	"\tpopl %esi\n"
	"\tpopl %ebx\n"
	"\tpopl %ebp\n"
	"\tpopl %ecx\n"
	"\tjmpl *%ecx\n"
#endif
);

#endif

ZEND_API zend_bool zend_fiber_switch_context(zend_fiber_context current, zend_fiber_context next)
{
	zend_fiber_context_posix *from;
	zend_fiber_context_posix *to;

	if (UNEXPECTED(current == NULL) || UNEXPECTED(next == NULL)) {
		return 0;
	}

	from = (zend_fiber_context_posix *) current;
	to = (zend_fiber_context_posix *) next;

	if (UNEXPECTED(from->initialized == 0) || UNEXPECTED(to->initialized == 0)) {
		return 0;
	}

#ifdef ZEND_FIBER_ASM
	zend_fiber_switch_context_asm(from, to);
#else
	if (swapcontext(&from->ctx, &to->ctx) == -1) {
		return 0;
	}
#endif

	return 1;
}

ZEND_API void zend_fiber_init()
{
#if __GCC_HAVE_DWARF2_CFI_ASM && __amd64
	asm (".cfi_undefined rip");
#endif
}

/*
 * vim: sw=4 ts=4
 * vim600: fdm=marker
 */
