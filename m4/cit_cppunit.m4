# -*- Autoconf -*-


# ======================================================================
# Autoconf macros for cppunit.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_CPPUNIT_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_CPPUNIT_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $CPPUNIT_INCLUDES"
  AC_LANG(C++)
  AC_REQUIRE_CPP
  AC_CHECK_HEADER([cppunit/TestRunner.h], [], [
    AC_MSG_ERROR([CppUnit header not found; try --with-cppunit-incdir=<CppUnit include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_CPPUNIT_HEADER


# ----------------------------------------------------------------------
# CIT_CPPUNIT_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_CPPUNIT_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $CPPUNIT_INCLUDES"
  LDFLAGS="$LDFLAGS $CPPUNIT_LDFLAGS"
  LIBS="-lcppunit"
  AC_LANG(C++)
  AC_REQUIRE_CPP
  AC_MSG_CHECKING([for CppUnit::TestRunner in -lcppunit])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[#include <cppunit/TestRunner.h>]],
	             [[CppUnit::TestRunner runner;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([CppUnit library not found; try --with-cppunit-libdir=<CppUnit lib dir>])
    ])
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_CPPUNIT_LIB


dnl end of file
