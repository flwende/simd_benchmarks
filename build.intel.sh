#!/bin/bash

source build_config.sh

build_config_knl

make -f makefile.intel clean
make -f makefile.intel $1 $2 $3
