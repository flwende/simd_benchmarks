#!/bin/bash

source build_config.sh

build_config_hsw

make -f makefile.intel.hsw clean
make -f makefile.intel.hsw $1 $2 $3
