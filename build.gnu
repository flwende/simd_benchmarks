#!/bin/bash

# export GLIBC_ROOT=${HOME}/opt/glibc-xxx
# export VC_ROOT=${HOME}/opt/Vc-yyy
# export VC_AVAILABLE=yes
export VC_AVAILABLE=no
export PLATFORM=x86

make -f makefile.gnu clean
make -f makefile.gnu -j16 $1 $2
