# -*- Autoconf -*-

# ======================================================================
# Autoconf macros for HDF5.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_HDF5_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_HDF5_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $HDF5_INCLUDES"
  AC_LANG(C)
  AC_REQUIRE_CPP
  AC_CHECK_HEADER([hdf5.h], [], [
    AC_MSG_ERROR([HDF5 header not found; try --with-hdf5-incdir=<hdf5 include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_HDF5_HEADER


# ----------------------------------------------------------------------
# CIT_NETCDF_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_HDF5_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $HDF5_INCLUDES"
  LDFLAGS="$LDFLAGS $HDF5_LDFLAGS"
  AC_LANG(C)
  AC_REQUIRE_CPP
  AC_CHECK_LIB(hdf5, H5Fopen, [],[
    AC_MSG_ERROR([HDF5 library not found; try --with-hdf5-libdir=<HDF5 lib dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_HDF5_LIB


# ----------------------------------------------------------------------
# CIT_NETCDF_LIB_PARALLEL
# ----------------------------------------------------------------------
AC_DEFUN([CIT_HDF5_LIB_PARALLEL], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $HDF5_INCLUDES"
  LDFLAGS="$LDFLAGS $HDF5_LDFLAGS"
  AC_LANG(C)
  AC_REQUIRE_CPP
  AC_CHECK_LIB(hdf5, H5Pset_dxpl_mpio, [],[
    AC_MSG_WARN([parallel HDF5 library not found; DO NOT attempt to use HDF5 in parallel OR configure HDF5 with '--enable-parallel'])
  ])
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_HDF5_LIB_PARALLEL


dnl end of file
