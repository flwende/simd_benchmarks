#!/bin/bash

#PBS -A # account for cpu time
#PBS -N simd_benchmarks_knl # job name

#PBS -l nodes=1:ppn=68 
#PBS -l os=CLE_quad_flat 
#PBS -l walltime=04:00:00

#xxPBS -o $SUBMISSIONDIR/results/$JOBNAME.o$JOBID
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

#export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH
#aprun -n 1 -N 1 -cc none ./benchmark.cray.knl.sh

aprun -n 1 -N 1 -cc none ./benchmark.intel.knl.sh
aprun -n 1 -N 1 -cc none ./benchmark.gnu.knl.sh
aprun -n 1 -N 1 -cc none ./benchmark.clang.knl.sh

