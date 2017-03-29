## ===========================================================================
## @file    Makefile.am
## @brief   automake configuration file for library
## @author  Tamas Dezso <dezso.t.tamas@gmail.com>
## @date    March 28, 2017
##
## Process this file with automake (autoreconf -i) to produce Makefile.in.
## ===========================================================================

ACLOCAL_AMFLAGS = -I m4 --install


lib_LTLIBRARIES = libstk.la
include_HEADERS = src/list.h src/stk.h
libstk_la_SOURCES = src/stk.c

#dist_doc_DATA = README.md


# Unit tests with cmocka (make check)
#if HAVE_CMOCKA
TESTS = $(check_PROGRAMS)
check_PROGRAMS = list_test stk_test

list_test_SOURCES = test/list_test.c
list_test_CFLAGS = -I$(top_srcdir)/src/
list_test_LDADD = -lcmocka

stk_test_SOURCES = test/stk_test.c
stk_test_CFLAGS = -I$(top_srcdir)/src/
stk_test_LDADD = -L$(top_builddir)/src/ -lstk -lcmocka
#endif


# Documentation with Doxygen
if HAVE_DOXYGEN

man_MANS = docs/man/man3/*.3

docs/man/man3/*.3: docs/doxyfile.stamp

docs/doxyfile.stamp: docs/Doxyfile
	cd docs; $(DOXYGEN)
	cd docs; touch doxyfile.stamp

clean-local:
	-rm -rf docs/man docs/html docs/latex docs/doxyfile.stamp

endif
