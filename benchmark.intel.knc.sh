#!/bin/bash

n=8192
curdir=`pwd`

for compiler in intel.knc
do
    for simdwidth in 8 16
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
		for threads in 1 2 120
		do
		    logfile=${benchdir}/benchmark_${executable}_${func}_simdwidth${simdwidth}_n${n}_threads${threads}.log
		    rm -f ${logfile}
		    for version in reference explicit_vectorization enhanced_explicit_vectorization manual_vectorization simd_class_vectorization
		    do
			echo "# version=${version}" >> ${logfile}
			if [[ "${threads}" == "120" ]]
			then
			    micnativeloadex ${benchdir}/${executable}_${version}.x -e "OMP_NUM_THREADS=${threads} KMP_AFFINITY=scatter,granularity=thread" -a "-n ${n}"
			else
			    micnativeloadex ${benchdir}/${executable}_${version}.x -e "OMP_NUM_THREADS=${threads} KMP_AFFINITY=compact,granularity=thread" -a "-n ${n}"
			fi
		    done
		done
	    done
	    cd ..
	done
    done
done
