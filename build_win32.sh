#!/bin/sh

set -xe


CC="tcc"
CFLAGS="-Wall -Wextra"
LFLAGS="-luser32 -lgdi32 -lkernel32"

$CC $CFLAGS -o game ./example_swl.c $LFLAGS
