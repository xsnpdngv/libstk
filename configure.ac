# ============================================================================
# @file    configure.ac
# @brief   autoconf configuration file to produce configure script
# @author  Tamas Dezso <dezso.t.tamas@gmail.com>
# @date    March 22, 2017
#
# Process this file with autoconf (autoreconf -i) to produce a
# configure script.
# ============================================================================

# initialize autoconf and automake
AC_PREREQ([2.69])
#AC_CONFIG_AUX_DIR([autotools])
AC_INIT([libstk], [1.0], [dezso.t.tamas@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/stk.c])
AC_CONFIG_HEADERS([config.h])

# check for programs
AC_PROG_CC([gcc])
AC_PROG_INSTALL
AC_PROG_RANLIB
AM_PROG_AR
# initialize libtool
LT_INIT

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])


#AC_CHECK_LIB([cmocka], [cmocka_run_one_tests])
#AC_SEARCH_LIBS([cmocka_run_group_tests_name], [cmocka])

# check for header files
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# check for typedefs, structures, and compiler characteristics
AC_TYPE_SIZE_T

# check for library functions
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
