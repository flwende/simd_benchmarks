#!/bin/bash

source build_config.sh

build_config_hsw

make -f makefile.cray.hsw clean
make -f makefile.cray.hsw $1 $2 $3
