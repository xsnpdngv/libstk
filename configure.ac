# ============================================================================
# @file    configure.ac
# @brief   autoconf configuration file to produce configure script
# @author  Tamas Dezso <dezso.t.tamas@gmail.com>
# @date    March 22, 2017
#
# Process this file with autoconf (or autoreconf -i) to produce a
# configure script.
# ============================================================================

AC_PREREQ([2.69])
AC_INIT([libstk], [1.0], [dezso.t.tamas@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# checks for programs
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

# checks for header files
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# checks for typedefs, structures, and compiler characteristics
AC_TYPE_SIZE_T

# checks for library functions
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
