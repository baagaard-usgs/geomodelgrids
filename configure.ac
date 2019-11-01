# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT([geomodelgrids], [0.1.0], [baagaard@usgs.gov])
AC_CONFIG_HEADER([portinfo])
AC_CONFIG_AUX_DIR([./aux-config])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])

# ----------------------------------------------------------------------
# Fortran
AC_ARG_ENABLE([fortran],
	[  --enable-fortran        Enable building Fortran bindings [[default=no]]],
	[enable_fortran=yes],
	[enable_fortran=no])

# TESTING w/cppunit
AC_ARG_ENABLE([testing],
	[  --enable-testing        Enable unit testing with cppunit (requires cppunit) [[default=no]]],
	[enable_testing=yes],
	[enable_testing=no])

# SPATIAL w/spatialdata extension
AC_ARG_ENABLE([spatial],
	[  --enable-spatial        Enable use of spatialdata package (requires spatialdata) [[default=no]]],
	[enable_spatial=yes],
	[enable_spatial=no])

# ----------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AC_PROG_LIBTOOL
if test "$allow_undefined_flag" = unsupported; then
    # See issue119.
    AM_LDFLAGS="-no-undefined $AM_LDFLAGS"
fi
AM_CONDITIONAL([NO_UNDEFINED], [test "$allow_undefined_flag" = unsupported])
AC_SUBST(AM_LDFLAGS)

# ----------------------------------------------------------------------
# PROJ4
AC_CHECK_LIB(proj, pj_init_plus, [
  AC_CHECK_HEADER([proj_api.h], [], [
    AC_MSG_ERROR([Proj4 header not found; try CPPFLAGS="-I<Proj4 include dir>"])
  ])
],[
  AC_MSG_ERROR([Proj4 library not found; try LDFLAGS="-L<Proj4 lib dir>"])
])

# FORTRAN BINDINGS
AM_CONDITIONAL([ENABLE_FORTRAN], [test "$enable_fortran" = yes])
if test "$enable_fortran" = "yes" ; then
  AC_PROG_FC
  F77 = FC
  AC_FC_WRAPPERS
  AC_FC_LIBRARY_LDFLAGS
fi

# CPPUNIT
AM_CONDITIONAL([ENABLE_TESTING], [test "$enable_testing" = yes])
if test "$enable_testing" = "yes" ; then
  AC_LANG(C++)
  AC_CHECK_HEADER([cppunit/TestRunner.h], [], [
    AC_MSG_ERROR([CppUnit header not found; try CPPFLAGS="-I<CppUnit include dir>"])
  ])
  AC_MSG_CHECKING([for CppUnit::TestRunner in -lcppunit])
  AC_REQUIRE_CPP
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <cppunit/TestRunner.h>]],
	             [[CppUnit::TestRunner runner;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([CppUnit library not found; try LDFLAGS="-L<CppUnit lib dir>"])
  ])
fi

# SPATIALDATA
AM_CONDITIONAL([ENABLE_SPATIAL], [test "$enable_spatial" = yes])
if test "$enable_spatial" = "yes" ; then
  AC_LANG(C++)
  AC_CHECK_HEADER([spatialdata/spatialdb/SpatialDB.hh], [], [
    AC_MSG_ERROR([SpatialDB header not found; try CPPFLAGS="-I<Spatialdata include dir>"])
])
  AC_MSG_CHECKING([for spatialdb::SimpleDB in -lspatialdata])
  AC_REQUIRE_CPP
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <spatialdata/spatialdb/SpatialDB.hh>]
                      [#include <spatialdata/spatialdb/SimpleDB.hh>]],
                     [[spatialdata::spatialdb::SimpleDB db;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([Spatialdata library not found; try LDFLAGS="-L<Spatialdata lib dir>"])
  ])
  if test "x$SPATIALDATA_SWIG_DIR" != "x" ; then
    AC_CHECK_FILE([$SPATIALDATA_SWIG_DIR/spatialdb/SpatialDBObj.i], [], [
      AC_MSG_ERROR([Spatialdata SpatialDBObj.i SWIG interface file not found])])
    AC_SUBST([SWIG_FLAGS], ["-I$SPATIALDATA_SWIG_DIR $SWIG_FLAGS"])
  else
    AC_CHECK_FILE([spatialdata/SpatialDBObj.i], [], [
      AC_MSG_ERROR([Spatialdata SpatialDBObj.i SWIG interface file not found; Try setting SPATIALDATA_SWIG_DIR=<directory containing spatialdb/SpatialDBObj.i>])])
  fi

  # PYTHON
  AM_PATH_PYTHON([3.6])
  CIT_PYTHON_SYSCONFIG

  # SWIG
  CIT_NUMPY_PYTHON_MODULE
  CIT_NUMPY_INCDIR
  AC_PROG_SWIG([2.0.4], [3.0.2])

fi

# ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
	libsrc/Makefile
	libsrc/geomodelgrids/Makefile
	libsrc/geomodelgrids/apps/Makefile
	libsrc/geomodelgrids/serial/Makefile
	bin/Makefile
	geomodelgrids/Makefile])

AC_OUTPUT


# End of file
