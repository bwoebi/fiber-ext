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

#ifndef FIBER_H
#define FIBER_H

#include "php.h"

typedef void* zend_fiber_context;

typedef void (* zend_fiber_func)();

ZEND_API zend_fiber_context zend_fiber_create_root_context();
ZEND_API zend_fiber_context zend_fiber_create_context();
ZEND_API zend_bool zend_fiber_create(zend_fiber_context context, zend_fiber_func func, size_t stack_size);
ZEND_API void zend_fiber_destroy(zend_fiber_context context);
ZEND_API zend_bool zend_fiber_switch_context(zend_fiber_context current, zend_fiber_context next);
ZEND_API void zend_fiber_init();

#endif

/*
 * vim: sw=4 ts=4
 * vim600: fdm=marker
 */
