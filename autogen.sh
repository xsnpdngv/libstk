#!/bin/sh
# ============================================================================
# @file    autogen.sh
# @brief   auto generation shell script to prepare Makefile
# @author  Tamas Dezso <dezso.t.tamas@gmail.com>
# @date    March 23, 2017
# ============================================================================

# Allow invocation from a separate build directory; in that case, we change
# to the source directory to run the auto*, then change back before running
# configure
srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir" || exit 1

# Test whether autotools are installed
if test -z `which autoreconf`; then
    echo "Error: autoreconf not found, please install it."
    exit 1
fi

# Generated system independent (portable) configuration script
autoreconf --force --verbose --install || exit $?

cd "$ORIGDIR" || exit 1

# Generate system specific Makefile with standard targets
./configure --prefix=$HOME
