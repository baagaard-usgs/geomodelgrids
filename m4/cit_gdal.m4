# -*- Autoconf -*-


# ======================================================================
# Autoconf macros for GDAL.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_GDAL_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_GDAL_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $PROJ_INCLUDES"
  AC_LANG(C++)
  AC_CHECK_HEADER([gdal/gdal.h], [], [
    AC_MSG_ERROR([GDAL header not found; try --with-gdal-incdir=<GDAL include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_GDAL_HEADER


# ----------------------------------------------------------------------
# CIT_GDAL_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_GDAL_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $GDAL_INCLUDES"
  LDFLAGS="$LDFLAGS $GDAL_LDFLAGS"
  AC_LANG(C++)
  AC_CHECK_LIB(gdal, GetGDALDriverManager, [],[
    AC_MSG_ERROR([GDAL library not found; try --with-gdal-libdir=<GDAL lib dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_GDAL_LIB


dnl end of file
