#!/bin/bash

source build_config.sh

build_config_knl

make -f makefile.intel.knl clean
make -f makefile.intel.knl $1 $2 $3
