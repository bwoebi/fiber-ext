PHP_ARG_ENABLE(fiber, whether to enable fiber support,
[  --enable-fiber          Enable fiber support], no)

if test "$PHP_FIBER" != "no"; then
  AC_DEFINE(HAVE_FIBER, 1, [ Have fiber support ])

  FIBER_CFLAGS="-z now -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1"

  AC_CHECK_HEADER(ucontext.h, [
    FIBER_CFLAGS+=" -DHAVE_UCONTEXT_H=1"
  ])
  
  LDFLAGS+="-z now"
  
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/valgrind/valgrind.h"
  for i in $SEARCH_PATH ; do
    if test -r $i/$SEARCH_FOR; then
      VALGRIND_DIR=$i
    fi
  done

  if test -z "$VALGRIND_DIR"; then
    AC_MSG_RESULT([not found])
  else
    AC_MSG_RESULT(found in $VALGRIND_DIR)
    AC_DEFINE(HAVE_VALGRIND, 1, [ ])
  fi
  
  PHP_NEW_EXTENSION(fiber, fiber_stack.c fiber_posix.c php_fiber.c, $ext_shared,, \\$(FIBER_CFLAGS))
  PHP_SUBST(FIBER_CFLAGS)
  PHP_ADD_MAKEFILE_FRAGMENT
fi
