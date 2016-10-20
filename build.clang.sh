#!/bin/bash

source build_config.sh

build_config

make -f makefile.clang clean
make -f makefile.clang $1 $2 $3
