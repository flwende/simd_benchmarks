#!/bin/bash

source build_config.sh

build_config_knl

make -f makefile.clang.knl clean
make -f makefile.clang.knl $1 $2 $3
