#!/bin/bash

source build_config.sh

build_config_knl

make -f makefile.gnu.knl clean
make -f makefile.gnu.knl $1 $2 $3
