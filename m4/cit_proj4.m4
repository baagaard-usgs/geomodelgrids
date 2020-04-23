# -*- Autoconf -*-


# ======================================================================
# Autoconf macros for Proj4.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_PROJ4_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ4_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $PROJ_INCLUDES"
  AC_LANG(C)
  AC_CHECK_HEADER([proj_api.h], [], [
    AC_MSG_ERROR([Proj header not found; try --with-proj-incdir=<Proj include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_PROJ4_HEADER


# ----------------------------------------------------------------------
# CIT_PROJ4_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ4_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $PROJ_INCLUDES"
  LDFLAGS="$LDFLAGS $PROJ_LDFLAGS"
  AC_LANG(C)
  AC_CHECK_LIB(proj, pj_init_plus, [],[
    AC_MSG_ERROR([Proj library not found; try --with-proj-libdir=<Proj lib dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_PROJ4_LIB


# ----------------------------------------------------------------------
# CIT_PROJ6_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ6_HEADER], [
  cit_save_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $PROJ_INCLUDES"
  AC_LANG(C)
  AC_CHECK_HEADER([proj.h], [], [
    AC_MSG_ERROR([Proj header (v6.x or later) not found; try --with-proj-incdir=<Proj include dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
])dnl CIT_PROJ6_HEADER


# ----------------------------------------------------------------------
# CIT_PROJ6_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ6_LIB], [
  cit_save_cppflags=$CPPFLAGS
  cit_save_ldflags=$LDFLAGS
  cit_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $PROJ_INCLUDES"
  LDFLAGS="$LDFLAGS $PROJ_LDFLAGS"
  AC_LANG(C)
  AC_CHECK_LIB(proj, proj_create_crs_to_crs, [],[
    AC_MSG_ERROR([Proj library (v6.x or later) not found; try --with-proj-libdir=<Proj lib dir>])
  ])dnl
  CPPFLAGS=$cit_save_cppflags
  LDFLAGS=$cit_save_ldflags
  LIBS=$cit_save_libs
])dnl CIT_PROJ6_LIB


dnl end of file
