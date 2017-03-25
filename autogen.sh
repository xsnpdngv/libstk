#!/bin/sh
# ============================================================================
# @file    autogen.sh
# @brief   auto generation shell script to prepare portable configure script
# @author  Tamas Dezso <dezso.t.tamas@gmail.com>
# @date    March 23, 2017
# ============================================================================

# Test whether autotools are installed
if test -z `which autoreconf`; then
    echo "Error: autoreconf not found, please install it."
    exit 1
fi

# Generated system independent (portable) configuration script
autoreconf --force --verbose --install || exit $?
echo "Done."
