PHP_ARG_ENABLE(fiber, whether to enable fiber support,
[  --enable-fiber          Enable fiber support], no)

if test "$PHP_FIBER" != "no"; then
  AC_DEFINE(HAVE_FIBER, 1, [ Have fiber support ])

  FIBER_CFLAGS="-z now -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1"

  AC_CHECK_HEADER(ucontext.h, [
    FIBER_CFLAGS+=" -DHAVE_UCONTEXT_H=1"
  ])
  
  LDFLAGS+="-z now"
  
  PHP_NEW_EXTENSION(fiber, fiber_stack.c fiber_posix.c php_fiber.c, $ext_shared,, \\$(FIBER_CFLAGS))
  PHP_SUBST(FIBER_CFLAGS)
  PHP_ADD_MAKEFILE_FRAGMENT
fi
