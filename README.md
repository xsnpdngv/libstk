libstk - Expanding stack C library
==================================

Lightweight singly linked list implementation and expanding stack library.
See header files under src for API documentation.


Clone to Get
------------

```bash
git clone https://github.com/xsnpdngv/libstk.git
```


Generate Configuration Script
-----------------------------

Autotools generate a portable script running which will result a target
machine compliant Makefile.

```bash
cd libstk/src
autoreconf -i
```


Compile and Install
-------------------

In order to install static library after building to the appropriate place
configure script needs the --prefix option to set. Use e.g., /usr/local to
have lib installed into /usr/local/includes/ and /usr/locab/lib. In case
having insufficient privileges to do so, choose e.g., the user home directory
`$HOME`.

```bash
mkdir -p build && cd build
../configure --prefix=$HOME
make
make install
```


Build Package
-------------

In order to build a portable source code tarball with a ready made configure
script make (and check) a distribution package:

```bash
make distcheck
```
