# Makefile for library testers

VPATH = ../src
CFLAGS = -Wall -I../src
LDLIBS = -lcmocka

.PHONY: testers
testers: list_test stk_test

stk.o: list.h stk.h
list_test: list.h
stk_test: stk.h stk.o

.PHONY: clean
clean:
	- rm -f *_test *.o
