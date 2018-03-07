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

#if defined(SIMD_CLASS_VCL)
	#include <vectorclass.h>
	#if defined(__MIC__) || defined(__AVX512F__)
		using VCL_Vec = Vec8d;
		using VCL_Mask = Vec8db;
	#elif defined(__AVX__) || defined(__AVX2__)
		using VCL_Vec = Vec4d;
		using VCL_Mask = Vec4db;
	#endif
#endif

#if defined(SIMD_CLASS_UMESIMD)
	#include <UMESimd.h>
	#if defined(__MIC__) || defined(__AVX512F__)
		using UMESIMD_Vec = UME::SIMD::SIMDVec_f<double, 8>;
		using UMESIMD_Mask = UME::SIMD::SIMDVecMask<8>;
	#elif defined(__AVX__) || defined(__AVX2__)
		using UMESIMD_Vec = UME::SIMD::SIMDVec_f<double, 4>;
		using UMESIMD_Mask = UME::SIMD::SIMDVecMask<4>;
	#endif
#endif

#include "common/simd_data_types.hpp"

// setup function macros to alias different math-function calls. 
#if defined(BENCHMARK_FUNC_SQRT_SQRT)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::sqrt(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::sqrt(X1)
	#define FUNC_2(X1) std::sqrt(X1)
	#define SIMD_FUNC_2(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_2(X1) Vc::sqrt(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_2(X1) UME::SIMD::FUNCTIONS::sqrt(X1)
#elif defined(BENCHMARK_FUNC_SQRT_LOG)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::sqrt(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::sqrt(X1)
	#define FUNC_2(X1) std::log(X1)
	#define SIMD_FUNC_2(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::sqrt(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::sqrt(X1)
#elif defined(BENCHMARK_FUNC_SQRT_EXP)
	#define FUNC_1(X1) std::sqrt(X1)
	#define SIMD_FUNC_1(X1) SIMD_SQRT_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_SQRT_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::sqrt(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::sqrt(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_2(X1) Vc::exp(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_2(X1) UME::SIMD::FUNCTIONS::exp(X1)
#elif defined(BENCHMARK_FUNC_LOG_LOG)
	#define FUNC_1(X1) std::log(X1)
	#define SIMD_FUNC_1(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::log(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::log(X1)
	#define FUNC_2(X1) std::log(X1)
	#define SIMD_FUNC_2(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_2(X1) Vc::log(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_2(X1) UME::SIMD::FUNCTIONS::log(X1)
#elif defined(BENCHMARK_FUNC_LOG_EXP)
	#define FUNC_1(X1) std::log(X1)
	#define SIMD_FUNC_1(X1) SIMD_LOG_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_LOG_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::log(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::log(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_2(X1) Vc::exp(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_2(X1) UME::SIMD::FUNCTIONS::exp(X1)
#elif defined(BENCHMARK_FUNC_EXP_EXP)
	#define FUNC_1(X1) std::exp(X1)
	#define SIMD_FUNC_1(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_1(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_1(X1) Vc::exp(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_1(X1) UME::SIMD::FUNCTIONS::exp(X1)
	#define FUNC_2(X1) std::exp(X1)
	#define SIMD_FUNC_2(X1) SIMD_EXP_REAL64(X1)
	#define SIMD_MASK_FUNC_2(X0, M1, X1) SIMD_MASK_EXP_REAL64(X0, M1, X1)
	#define SIMD_CLASS_VC_FUNC_2(X1) Vc::exp(X1)
	#define SIMD_CLASS_UMESIMD_FUNC_2(X1) UME::SIMD::FUNCTIONS::exp(X1)
#endif

// reference kernel prototype.
void kernel_reference(const double& x_1, const double& x_2, double& y);

#if defined(EXPLICIT_VECTORIZATION)
	// kernel prototypes using OpenMP 4.x omp declare simd directive.
	#pragma omp declare simd simdlen(SIMD_WIDTH_LOGICAL_REAL64) linear(x_1 : 1) linear(x_2 : 1) linear(y : 1)
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
#elif defined(SIMD_CLASS_UMESIMD)
	// simd class vectorization with UMESIMD.
	void kernel_simd_class_umesimd(const UMESIMD_Vec& x_1, const UMESIMD_Vec& x_2, UMESIMD_Vec& y, UMESIMD_Mask& m);
#endif

#endif
