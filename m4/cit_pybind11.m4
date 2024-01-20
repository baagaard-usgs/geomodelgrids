# -*- Autoconf -*-


## --------------------------- ##
## Autoconf macros for pybind11. ##
## --------------------------- ##


# CIT_PYTHON_INCDIR
# -----------------
# Determine the directory containing <Python.h> using distutils.
AC_DEFUN([CIT_PYTHON_INCDIR], [
# $Id$
AC_REQUIRE([AM_PATH_PYTHON])
AC_CACHE_CHECK([for $am_display_PYTHON include directory],
    [_cv_PYTHON_INCDIR],
    [_cv_PYTHON_INCDIR=`$PYTHON -c "from distutils import sysconfig; print(sysconfig.get_python_inc())" 2>/dev/null ||
     echo "$PYTHON_PREFIX/include/python$PYTHON_VERSION"`])
AC_SUBST([PYTHON_INCDIR], [$_cv_PYTHON_INCDIR])
])dnl CIT_PYTHON_INCDIR


# CIT_CHECK_PYTHON_HEADER
# -----------------------
# Checking the existence of Python.h
AC_DEFUN([CIT_CHECK_PYTHON_HEADER], [
# $Id$
AC_REQUIRE([CIT_PYTHON_INCDIR])
cit_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="-I$PYTHON_INCDIR $cit_save_CPPFLAGS"
AC_CHECK_HEADER([Python.h], [], [
                AC_MSG_ERROR([Header file 'Python.h' not found; maybe you don't have the python development package, e.g. 'python-dev', installed?])
                ])
CPPFLAGS=$cit_save_CPPFLAGS
])dnl CIT_CHECK_PYTHON_HEADER


# CIT_CHECK_PYTHON_SHARED
# -----------------------
# Check whether -lpythonX.X is a shared library.
AC_DEFUN([CIT_CHECK_PYTHON_SHARED], [
# $Id$
AC_REQUIRE([CIT_PYTHON_CONFIG])
AC_MSG_CHECKING([whether -lpython$PYTHON_VERSION is a shared library])
cit_save_CPPFLAGS=$CPPFLAGS
cit_save_LDFLAGS=$LDFLAGS
cit_save_LIBS=$LIBS
CPPFLAGS="$PYTHON_CPPFLAGS $cit_save_CPPFLAGS"
LDFLAGS="$PYTHON_LDFLAGS $cit_save_LDFLAGS"
LIBS="$PYTHON_LIBS $cit_save_LIBS"
AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include "Python.h"
]], [[
    int status;
    Py_Initialize();
    status = PyRun_SimpleString("import binascii") != 0;
    Py_Finalize();
    return status;
]])], [
    AC_MSG_RESULT(yes)
], [
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([-lpython$PYTHON_VERSION is not a shared library])
])
CPPFLAGS=$cit_save_CPPFLAGS
LDFLAGS=$cit_save_LDFLAGS
LIBS=$cit_save_LIBS
])dnl CIT_CHECK_PYTHON_SHARED


# CIT_PYBIND11_INCLUDES
# -----------------
AC_DEFUN([CIT_PYBIND11_INCLUDES], [
AC_REQUIRE([AM_PATH_PYTHON])
AC_MSG_CHECKING([$am_display_PYTHON pybind11])
  PYBIND11_CPPFLAGS=`$PYTHON -m pybind11 --includes`
if test -n "$PYBIND11_CPPFLAGS"; then
    AC_MSG_RESULT(ok)
else
    AC_MSG_ERROR(["failed

Run '$PYTHON -m pybind11 --includes' to see what went wrong.
"])
fi
AC_SUBST([PYBIND11_CPPFLAGS], [$PYBIND11_CPPFLAGS])
])dnl CIT_PYBIND11_INCLUDES



dnl end of file
