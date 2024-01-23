# -*- Autoconf -*-


# ======================================================================
# Autoconf macros for catch2.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_CATCH2_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_CATCH2_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $CATCH2_INCLUDES"
  AC_LANG(C++)
  AC_REQUIRE_CPP
  AC_CHECK_HEADER([catch2/catch_test_macros.hpp], [], [
    AC_MSG_ERROR([Catch2 header not found; try --with-catch2-incdir=<Catch2 include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_CATCH2_HEADER


# ----------------------------------------------------------------------
# CIT_CATCH2_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_CATCH2_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $CATCH2_INCLUDES"
  LDFLAGS="$LDFLAGS $CATCH2_LDFLAGS"
  LIBS="-lCatch2"
  AC_LANG(C++)
  AC_REQUIRE_CPP
  AC_MSG_CHECKING([for REQUIRE() in -lCatch2])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[#include <catch2/catch_test_macros.hpp>]],
	             [[REQUIRE(1==1)]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([Catch2 library not found; try --with-catch2-libdir=<Catch2 lib dir>])
    ])
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_CATCH2_LIB


dnl end of file
