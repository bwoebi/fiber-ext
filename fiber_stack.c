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

#include "fiber_stack.h"

zend_bool zend_fiber_stack_allocate(zend_fiber_stack *stack, unsigned int size)
{
	static __thread size_t page_size;

	if (!page_size) {
		page_size = ZEND_FIBER_PAGESIZE;
	}

	stack->size = ((size_t) size * sizeof(void *) + page_size - 1) / page_size * page_size;

#ifdef ZEND_FIBER_MMAP

	void *pointer;
	size_t msize;

	msize = stack->size + ZEND_FIBER_GUARDPAGES * page_size;

	pointer = mmap(0, msize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (pointer == (void *) -1) {
		pointer = mmap(0, msize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (pointer == (void *) -1) {
			return 0;
		}
	}

#if ZEND_FIBER_GUARDPAGES
	mprotect(pointer, ZEND_FIBER_GUARDPAGES * page_size, PROT_NONE);
#endif

	stack->pointer = (void *)((char *) pointer + ZEND_FIBER_GUARDPAGES * page_size);
#else
	stack->pointer = emalloc(stack->size);
#endif

	if (!stack->pointer) {
		return 0;
	}

	return 1;
}

void zend_fiber_stack_free(zend_fiber_stack *stack)
{
	static __thread size_t page_size;

	if (!page_size) {
		page_size = ZEND_FIBER_PAGESIZE;
	}

	if (stack->pointer != NULL) {
#ifdef ZEND_FIBER_MMAP

		void *address;
		size_t len;

		address = (void *)((char *) stack->pointer - ZEND_FIBER_GUARDPAGES * page_size);
		len = stack->size + ZEND_FIBER_GUARDPAGES * page_size;

		munmap(address, len);
		stack->pointer = NULL;
#else
		efree(stack->pointer);
		stack->pointer = NULL;
#endif
	}
}

/*
 * vim: sw=4 ts=4
 * vim600: fdm=marker
 */
