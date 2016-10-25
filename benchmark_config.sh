#!/bin/bash

function benchmark_config {
	curdir=`pwd`
	n=8192
	funcs="SQRT_SQRT LOG_LOG EXP_EXP"
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

function benchmark_config_hsw {
	simdwidths="4 8"
	threads="1 12 24"
}

function benchmark_config_knl {
	simdwidths="8 16"
	threads="1 64 68"
}

