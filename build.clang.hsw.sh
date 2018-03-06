#!/bin/bash

source build_config.sh

build_config_hsw

make -f makefile.clang.hsw clean
make -f makefile.clang.hsw $1 $2 $3 $4
