#!/bin/bash

n=8192
curdir=`pwd`

for compiler in gnu
do
    for simdwidth in 4 8
    do
	for func in SQRT_SQRT LOG_LOG EXP_EXP
	do
	    ${curdir}/build.${compiler} FUNC=${func} SIMDWIDTH=${simdwidth}
	    benchdir=${curdir}/benchmark.${compiler}
	    mkdir -p ${benchdir}
	    cp bin.${compiler}/*.x ${benchdir}
	    cp run.${compiler} ${benchdir}
	    cd ${benchdir}
	    for executable in simple conditional_math_call early_return nested_branching while_loop
	    do
		for threads in 1 12
		do
		    export OMP_NUM_THREADS=${threads}
		    logfile=${benchdir}/benchmark_${executable}_${func}_simdwidth${simdwidth}_threads${threads}_n${n}.log
		    rm -f ${logfile}
		    for version in reference explicit_vectorization enhanced_explicit_vectorization manual_vectorization simd_class_vectorization
		    do
			echo "# version=${version}" >> ${logfile}
			${benchdir}/${executable}_${version}.x -n ${n} >> ${logfile}
		    done
		done
	    done
	    cd ..
	done
    done
done
