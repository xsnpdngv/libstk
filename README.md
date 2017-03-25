Expanding Stack library
=======================

Lightweight singly linked list implementation and expanding stack library.
See header files under `src` for API documentation.

Install
-------

Either you can download the latest release as a portable source tarball,
or clone the repo and autogenerate what is needed to build it.

### 1.A., Download

To install `libstk` from source tarball, download latest tar.gz from
[github.com/xsnpdngv/libstk/releases](https://github.com/xsnpdngv/libstk/releases).

```bash
wget https://github.com/xsnpdngv/libstk/releases/download/v1.0/libstk-1.0.tar.gz
tar xzvf libstk-1.0.tar.gz
cd libstk-1.0
```

### 1.B., Clone

To build from the repository itself, it is needed to have GNU
autotools installed. Through the `autogen.sh` script this will generate
a portable `configure` script running which results system specific Makefile.

```bash
git clone https://github.com/xsnpdngv/libstk.git
cd libstk
./autogen.sh
```

### 2., Configure and make

After the previous step the system specific configuration follows. In order
to have the library installed to a non-standard place (e.g., due to
insufficient privileges), an alternative location can be provided to
the configuration phase via the `--prefix` option.

```bash
./configure --prefix=$HOME
make
make install
```

### [ 3., Build distribution package ]

In order to (re)generate the distribution tarball the `distcheck`
make target needs to be built.

```bash
make distcheck
```

Usage
-----

After installation, the library can be used just like any standard one.

The program have to include the header and then can use the lib.

```c
#include <stdio.h>
#include <stk.h>

int main(int argc, char **argv)
{
    stk_t *s = stkNew(128);
    int i;

    for(i = 0; i < 10; i++)
        stkPushInt(s, i);

    while(!stkIsEmpty(s))
    {
        printf("%s\n", stkValToStr(s));
        stkPop(s);
    }

    stkDestroy(s);
    return 0;
}
```

The compiler have to know where to find this header if not in a standard
location. The option `-I` can be used to give it e.g., `-I$(HOME)/include`.

The linker needs the library to link it together with the program, so
`-lstk` have to be provided. To find it in a non usual place, the option
`-L` is also needed to name it e.g., `-L$(HOME)/lib`.

An example Makefile utilizing all the above, would look like below.

```Makefile
CFLAGS  = -I$(HOME)/include
LDFLAGS = -L$(HOME)/lib
LDLIBS  = -lstk

program:
```

See the `examples` directory for samples.
