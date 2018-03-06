#!/bin/bash

source build_config.sh

build_config_knl

#export UMESIMD_AVAILABLE=no

make -f makefile.cray.knl clean
make -f makefile.cray.knl $1 $2 $3 $4
