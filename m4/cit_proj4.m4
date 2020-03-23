# -*- Autoconf -*-


# ======================================================================
# Autoconf macros for Proj4.
# ======================================================================

# ----------------------------------------------------------------------
# CIT_PROJ4_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ4_HEADER], [
  AC_LANG(C)
  AC_CHECK_HEADER([proj_api.h], [], [
    AC_MSG_ERROR([Proj4 header not found; try CPPFLAGS="-I<Proj4 include dir>"])
  ])dnl
])dnl CIT_PROJ4_HEADER


# ----------------------------------------------------------------------
# CIT_PROJ4_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ4_LIB], [
  AC_LANG(C)
  AC_CHECK_LIB(proj, pj_init_plus, [],[
    AC_MSG_ERROR([Proj4 library not found; try LDFLAGS="-L<Proj4 lib dir>"])
  ])dnl
])dnl CIT_PROJ4_LIB


# ----------------------------------------------------------------------
# CIT_PROJ6_HEADER
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ6_HEADER], [
  AC_LANG(C)
  AC_CHECK_HEADER([proj.h], [], [
    AC_MSG_ERROR([Proj header (v6.x or later) not found; try CPPFLAGS="-I<Proj include dir>"])
  ])dnl
])dnl CIT_PROJ6_HEADER


# ----------------------------------------------------------------------
# CIT_PROJ6_LIB
# ----------------------------------------------------------------------
AC_DEFUN([CIT_PROJ6_LIB], [
  AC_LANG(C)
  AC_CHECK_LIB(proj, proj_create_crs_to_crs, [],[
    AC_MSG_ERROR([Proj library (v6.x or later) not found; try LDFLAGS="-L<Proj lib dir>"])
  ])dnl
])dnl CIT_PROJ6_LIB


dnl end of file
