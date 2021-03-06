Expanding Stack C library
=========================

Lightweight singly linked list implementation and expanding stack library.
See header files under `src` for API documentation.


Install
-------

Either you can download the latest release as a portable source tarball,
or clone the repo and autogenerate prior to configure, build and install.
Alternatively you can just copy and use the sources.

### 1/A. Download tarball

To install `libstk` library from source tarball, download latest tar.gz from
[github.com/xsnpdngv/libstk/releases](https://github.com/xsnpdngv/libstk/releases).

```bash
wget https://github.com/xsnpdngv/libstk/releases/download/v1.0/libstk-1.0.tar.gz
tar xzvf libstk-1.0.tar.gz
cd libstk-1.0
```

### 1/B. Clone repo

To build from the repository itself, it is needed to have GNU
autotools installed. Through the `autogen.sh` script this will generate
a portable `configure` script running which results system specific Makefile.

```bash
git clone https://github.com/xsnpdngv/libstk.git
cd libstk
./autogen.sh
```

### 1/C. Copy source

Naturally it is an option too to simply copy out the sources from `src`
and compile them together with your project.

### 2. Configure, build and install

After the previous step the system specific configuration follows. In order
to have the library installed to a non-standard place (due to e.g.,
insufficient privileges), an alternative location can be provided in
the configuration phase via the `--prefix` option.

```bash
./configure --prefix=$HOME
make
make install
```

In this case headers go to `$HOME/include` and the compiled library object
goes to `$HOME/lib`.

### [ 3. Optional ]

With `make check` the library unit tests can be run to check
functionality, however this step requires the
[cmocka unit testing framework](https://cmocka.org) to be previously installed.

```bash
# cmake
sudo apt-get install cmake
# cmocka unit testing framework
CMVER=1.1.0
curl -R -O https://cmocka.org/files/1.1/cmocka-${CMVER}.tar.xz
unxz cmocka-${CMVER}.tar.xz
tar xvf cmocka-${CMVER}.tar
cd cmocka-${CMVER}
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug ..
make
sudo make install
```

With `make distcheck` the distribution tarball can be (re)generated.

With `make uninstall` the package can be safely removed from the install
location.


Usage
-----

After installation, the library can be used just like any standard one.

### Include

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

### Compile

The compiler have to know where to find the header if not in a standard
location. The option `-I` can be used to let it know.

```bash
cc -I$HOME/include stk_eg.c -c
```

### Link

The linker needs the library to link it together with your program, so
`-lstk` has to be provided. To find it in a non usual place, the option
`-L` is also needed to specify it.

```bash
cc -L$HOME/lib stk_eg.o -lstk -o stk_eg
```

### Makefile

To use the library through a Makefile the followings would be needed.

```Makefile
CFLAGS  = -I$(HOME)/include
LDFLAGS = -L$(HOME)/lib
LDLIBS  = -lstk
```

See the `examples` directory for samples.
