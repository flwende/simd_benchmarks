#!/bin/bash

function benchmark {
	local curdir=$1
	local n=$2
	local compiler=$3
	local simdwidths=$4 # NOTE: quote for spaces
	local funcs=$5 # NOTE: quote for spaces
	local threads=$6 # NOTE: quote for spaces
	local executables=$7 # NOTE: quote for spaces
	local versions=$8 # NOTE: quote for spaces
	
# e.g.:
#	curdir=`pwd`
#	n=8192
#	compiler=cray
#	simdwidths="4 8"
#	funcs="SQRT_SQRT LOG_LOG EXP_EXP"
#	threads="1 12"
#	executables="simple conditional_math_call early_return nested_branching while_loop"
#	versions="reference explicit_vectorization enhanced_explicit_vectorization intrinsics simd_class_vc"
	

	local suffix=$(date +%Y-%m-%d_%H%M%S)
	local benchdir=${curdir}/results/benchmark.${compiler}.${suffix}
	mkdir -p ${benchdir}
	local bindir=${benchdir}/bin
	mkdir -p ${bindir}

    for simdwidth in $simdwidths
    do
		for func in $funcs
		do
			# build
			${curdir}/build.${compiler}.sh FUNC=${func} SIMDWIDTH=${simdwidth} BIN_DIR=${bindir}
			cd ${benchdir}

			for executable in $executables
			do
				for thread in $threads
				do
					logfile=${benchdir}/benchmark_${executable}_${func}_simdwidth${simdwidth}_threads${thread}_n${n}.log
					rm -f ${logfile}
					for version in $versions
					do
						echo "# version=${version}" >> ${logfile}
						# run command
						OMP_NUM_THREADS=${thread} OMP_PLACES=threads OMP_PROC_BIND=true ${bindir}/${executable}_${version}.x -n ${n} >> ${logfile}
					done
				done
			done

			cd ..
		done
    done
}

