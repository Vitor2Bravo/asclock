#!/usr/bin/env bash

set -xe

CC="cc"
OPT="-Wall -g"
BIN="asclock"
SRC="main.c utils.c"
SRC2="utils.c"

if [ $# -eq 0 ]
then
	$CC $OPT "$SRC" -o "$BIN"
fi

if [ "$1" == "run" ]
then
	./"$BIN"
fi

if [ "$1" == "clean" ]
then
	rm -f *.o "$BIN"
fi
