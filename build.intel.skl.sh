#!/bin/bash

source build_config.sh

build_config_skl

make -f makefile.intel.skl clean
make -f makefile.intel.skl $1 $2 $3 $4
