#!/bin/bash

source build_config.sh

build_config

make -f makefile.cray clean
make -f makefile.cray $1 $2 $3
