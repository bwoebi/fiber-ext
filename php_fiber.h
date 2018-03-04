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
  | Authors: Haitao Lv <php@lvht.net>                                    |
  |          Martin Schröder <m.schroeder2007@gmail.com>                 |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_FIBER_H
#define PHP_FIBER_H

extern zend_module_entry fiber_module_entry;
#define phpext_fiber_ptr &fiber_module_entry

#define PHP_FIBER_VERSION "0.1.0"

#if defined(ZTS) && defined(COMPILE_DL_FIBER)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

static const zend_uchar ZEND_FIBER_STATUS_INIT = 0;
static const zend_uchar ZEND_FIBER_STATUS_SUSPENDED = 1;
static const zend_uchar ZEND_FIBER_STATUS_RUNNING = 2;
static const zend_uchar ZEND_FIBER_STATUS_FINISHED = 3;
static const zend_uchar ZEND_FIBER_STATUS_DEAD = 4;

#define REGISTER_FIBER_CLASS_CONST_LONG(const_name, value) \
	zend_declare_class_constant_long(zend_ce_fiber, const_name, sizeof(const_name)-1, (zend_long)value);

typedef void* zend_fiber_context;

typedef struct _zend_fiber zend_fiber;

struct _zend_fiber {
	/* Fiber PHP object handle. */
	zend_object std;

	/* Status of the fiber, one of the ZEND_FIBER_STATUS_* constants. */
	zend_uchar status;

	/* Callback and info / cache to be used when fiber is started. */
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	/* Fiber context of this fiber, will be created during call to start(). */
	zend_fiber_context context;

	/* Fiber context of the fiber that called start(), resume() or throw(). */
	zend_fiber_context caller;

	/* PHP value being passed into or returned from the fiber. */
	zval value;

	/* Current Zend VM execute data being run by the fiber. */
	zend_execute_data *exec;

	/* VM stack and stack size being used by the fiber. */
	zend_vm_stack stack;
	size_t stack_size;
};

ZEND_BEGIN_MODULE_GLOBALS(fiber)
	/* Root fiber context (main thread). */
	zend_fiber_context root;

	/* Active fiber, NULL when in main thread. */
	zend_fiber *current_fiber;

	/* Default fiber stack size. */
	zend_long stack_size;

	/* Error to be thrown into a fiber (will be populated by throw()). */
	zval *error;
ZEND_END_MODULE_GLOBALS(fiber)

#define FIBER_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(fiber, v)

#endif

/*
 * vim: sw=4 ts=4
 * vim600: fdm=marker
 */
