#!/bin/bash

source build_config.sh

build_config

make -f makefile.gnu clean
make -f makefile.gnu $1 $2 $3
