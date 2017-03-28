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

# unit tests with cmocka (make check)
check_PROGRAMS = list_test stk_test

list_test_SOURCES = test/list_test.c
list_test_CFLAGS = -Isrc/
list_test_LDADD = -lcmocka

stk_test_SOURCES = test/stk_test.c
stk_test_CFLAGS = -Isrc/
stk_test_LDADD = libstk.la -lcmocka

TESTS = $(check_PROGRAMS)
