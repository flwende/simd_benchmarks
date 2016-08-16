#!/bin/bash

# export GLIBC_ROOT=${HOME}/opt/glibc-xxx
# export VC_ROOT=${HOME}/opt/Vc-yyy
# export VC_AVAILABLE=yes
export VC_AVAILABLE=no
export PLATFORM=mic

make -f makefile.intel.knc clean
make -f makefile.intel.knc $1 $2
