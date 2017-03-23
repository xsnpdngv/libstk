libstk
======

Lightweight singly linked list implementation and expanding stack library.
See header files under src for API documentation.

Prerequisites
-------------

It is needed to have GNU autotools installed for building libstk.

```bash
sudo apt-get install autoconf automake
```

Build
-----

Run `autogen.sh` in order to generate the system specific Makefile through
invoking the self generated configure script. Once the `autogen.sh` is run and
so Makefile is resulted, the library can be compiled and installed.

```bash
./autogen.sh
make
make install
```

By default `autogen.sh` runs the `configure` script with --prefix=$HOME, so
on `make install` the target directories are `~/include` and `~/lib`.
If other prefix is needed, edit `autogen.sh` prior to run. If a place
is choosed that requires superuser privileges, issue `sudo make install`
at last.

In order to build a portable source code tarball with a ready made configure
script make (and check) a distribution package:

```bash
make distcheck
make distclean
```
