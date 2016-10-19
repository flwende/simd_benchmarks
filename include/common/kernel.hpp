// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#if !defined(KERNEL_HPP)
#define KERNEL_HPP

#include <cstdint>
#include <cmath>

#if defined(SIMD_CLASS_VC)
	#include <Vc/Vc>
#endif

#include "common/simd_data_types.hpp"

// setup function macros to alias different math-function calls. 
#if defined(BENCHMARK_FUNC_SQRT_SQRT)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::sqrt(X1)
	#define FUNC_2(X1) std::sqrt(X1)
	#define SIMD_FUNC_2(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::sqrt(X1)
#elif defined(BENCHMARK_FUNC_SQRT_LOG)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::sqrt(X1)
	#define FUNC_2(X1) std::log(X1)
	#define SIMD_FUNC_2(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::log(X1)
#elif defined(BENCHMARK_FUNC_SQRT_EXP)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::sqrt(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::exp(X1)
#elif defined(BENCHMARK_FUNC_LOG_LOG)
	#define FUNC_1(X1) std::log(X1)
	#define SIMD_FUNC_1(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::log(X1)
	#define FUNC_2(X1) std::log(X1)
	#define SIMD_FUNC_2(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::log(X1)
#elif defined(BENCHMARK_FUNC_LOG_EXP)
	#define FUNC_1(X1) std::log(X1)
	#define SIMD_FUNC_1(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::log(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::exp(X1)
#elif defined(BENCHMARK_FUNC_EXP_EXP)
	#define FUNC_1(X1) std::exp(X1)
	#define SIMD_FUNC_1(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_1(X1) Vc::exp(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_FUNC_2(X1) Vc::exp(X1)
#endif

// reference kernel prototype.
void kernel_reference(const double& x_1, const double& x_2, double& y);

#if defined(EXPLICIT_VECTORIZATION)
	// kernel prototypes using OpenMP 4.x omp declare simd directive.
	#if defined(__CLANG_COMPILER__)
		// currently not supported by clang.
	#elif defined(__INTEL_COMPILER)
		// there is seemingly a bug with the Intel compiler 16.x that requires to use multiple
		// linear and aligned clauses if there is more than one variable.
		#pragma omp declare simd simdlen(SIMD_WIDTH_LOGICAL_REAL64) linear(x_1 : 1) linear(x_2 : 1) linear(y : 1) aligned(x_1 : ALIGNMENT) aligned(x_2 : ALIGNMENT) aligned(y : ALIGNMENT)
	#else
		// specifying the aligned clause gives a compile error with GNU compilers 5.x.
		// the function definition, however, uses the aligned clause.
		#pragma omp declare simd simdlen(SIMD_WIDTH_LOGICAL_REAL64) linear(x_1 : 1) linear(x_2 : 1) linear(y : 1)
	#endif
	// simd function definition works with pointer data types only when using the GNU compiler (Intel also can handle reference parameters)
	void kernel_explicit_vectorization(const double*__restrict__ x_1, const double*__restrict__ x_2, double*__restrict__ y);
#elif defined(ENHANCED_EXPLICIT_VECTORIZATION)
	// kernel using OpenMP 4.x directives in its body.
	void kernel_enhanced_explicit_vectorization(const vec_real64_t& x_1, const vec_real64_t& x_2, vec_real64_t& y, const mask_real64_t& m);
#elif defined(INTRINSICS)
	// manual vectorization.
	void kernel_intrinsics(const VEC_REAL64& x_1, const VEC_REAL64& x_2, VEC_REAL64& y, const MASK_REAL64& m);
#elif defined(SIMD_CLASS_VC)
	// simd class vectorization with VC.
	void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& m);
#endif

#endif
