#!/bin/bash

source build_config.sh

build_config_hsw

make -f makefile.gnu.hsw clean
make -f makefile.gnu.hsw $1 $2 $3 $4
