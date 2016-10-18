#!/bin/bash

#PBS -A # account for cpu time
#PBS -N simd_benchmarks_hsw # job name

#PBS -q mpp2q
#PBS -l nodes=4:ppn=24 
#PBS -l walltime=04:00:00

#xxPBS -o results/$JOBNAME.o$JOBID
#PBS -j oe # join error into output


# cray compiler
module switch cce/8.5.3

# intel compiler
module load intel/17.0.0.098

# gcc
export PATH=$HOME/Software/gcc-6.2.0/bin:$PATH
export LD_LIBRARY_PATH=$HOME/Software/gcc-6.2.0/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HOME/Software/gcc-6.2.0/lib/:$LD_LIBRARY_PATH

# clang
export PATH=$HOME/Software/llvm_3.9.0/bin:$PATH
export LD_LIBRARY_PATH=$HOME/Software/llvm_3.9.0/lib:$LD_LIBRARY_PATH

module switch craype-ivybridge craype-haswell

aprun -n 1 -N 1 -cc none ./benchmark.cray.sh
aprun -n 1 -N 1 -cc none ./benchmark.intel.sh
aprun -n 1 -N 1 -cc none ./benchmark.gnu.sh
aprun -n 1 -N 1 -cc none ./benchmark.clang.sh

