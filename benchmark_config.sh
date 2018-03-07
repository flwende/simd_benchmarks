#!/bin/bash

function benchmark_config {
	curdir=`pwd`
	n=8192
	#funcs="SQRT_SQRT LOG_LOG EXP_EXP"
	funcs="SQRT_SQRT EXP_EXP"
	executables="simple conditional_math_call early_return nested_branching while_loop"
	versions="reference explicit_vectorization enhanced_explicit_vectorization intrinsics simd_class_umesimd" # simd_class_vc 
}

function benchmark_config_no_intrin {
	curdir=`pwd`
	n=8192
	funcs="SQRT_SQRT LOG_LOG EXP_EXP"
	executables="simple conditional_math_call early_return nested_branching while_loop"
	versions="reference explicit_vectorization enhanced_explicit_vectorization"
}

# 2x Intel Xeon E5-2630 v3 (Haswell, HSW, 2.4 Ghz, 8 cores * 2 Hyperthreads)
function benchmark_config_hsw {
	benchmark_config
	simdwidths="4 8" # AVX2
	threads="1 16 32"
}

# 1x Intel Xeon Phi 7210 (Knights Landing, KNL, 1.3 GHz, 64 cores, 4 Threads)
function benchmark_config_knl {
	benchmark_config
	simdwidths="8 16" # AVX-512
	threads="1 64 128" # max. 2 threads
}

# 2x	Intel Xeon Gold 6138 CPU (Skylake, SKL, 2.0 GHz, 20 cores, 2 Threads)
function benchmark_config_skl {
	benchmark_config
	simdwidths="8 16" # AVX-512
	threads="1 40 80"
}

