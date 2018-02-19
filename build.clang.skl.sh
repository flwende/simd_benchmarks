#!/bin/bash

source build_config.sh

build_config_knl

make -f makefile.clang.skl clean
make -f makefile.clang.skl $1 $2 $3
