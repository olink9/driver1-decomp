#!/bin/bash
# usage: dump.sh file.c flags...   -> dumps in $W/dump/
cd "$(dirname "$0")/../.."
W=${DUMP_DIR:-build/dump0-$$}; mkdir -p $W   # per-worker dir: set DUMP_DIR
mkdir -p $W/dump; rm -f $W/dump/*
f=$1; shift
compilers/gcc-2.7.2-psx/cpp -Iinclude -Iinclude/shim -Iinclude/psyq -Isrc -undef -Wall -lang-c -nostdinc -Dmips -D__GNUC__=2 -D__OPTIMIZE__ -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C -DDRIVER1 $f $W/dump/x.i
compilers/gcc-2.7.2-cdk/cc1 -O2 -G0 -mips1 -mcpu=3000 -fgnu-linker -mgas -msoft-float -quiet -funsigned-char -w "$@" $W/dump/x.i -o $W/dump/x.s
