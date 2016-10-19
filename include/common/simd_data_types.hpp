// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#if !defined(SIMD_DATA_TYPES_HPP)
#define SIMD_DATA_TYPES_HPP

#include <cmath>
#include <cstdint>
#include <immintrin.h>

#if defined(__MIC__) || defined(__AVX512F__)
	#define SIMD_512
	#define ALIGNMENT (64)
	// the simdwidth given by the native size of the simd registers: for 64-bit words.
	#define SIMD_WIDTH_NATIVE_REAL64 (8)
	// OpenMP 4.x allows to operate on logical vectors of larger size.
	// values here can be larger than SIMD_WIDTH_NATIVE_REAL64.
	#if defined(MANUAL_VECTORIZATION)
		#undef SIMD_WIDTH_LOGICAL_REAL64
		#define SIMD_WIDTH_LOGICAL_REAL64 (8)
	#else
		#if !defined(SIMD_WIDTH_LOGICAL_REAL64)
			#define SIMD_WIDTH_LOGICAL_REAL64 (8)
		#endif
	#endif
#elif defined(__AVX__) || defined(__AVX2__)
	#define SIMD_256
	#define ALIGNMENT (32)
	// the simdwidth given by the native size of the simd registers: for 64-bit words.
	#define SIMD_WIDTH_NATIVE_REAL64 (4)
	// OpenMP 4.x allows to operate on logical vectors of larger size.
	// values here can be larger than SIMD_WIDTH_NATIVE_REAL64.
	#if defined(MANUAL_VECTORIZATION)
		#undef SIMD_WIDTH_LOGICAL_REAL64
		#define SIMD_WIDTH_LOGICAL_REAL64 (4)
	#else
		#if !defined(SIMD_WIDTH_LOGICAL_REAL64)
			#define SIMD_WIDTH_LOGICAL_REAL64 (4)
		#endif
	#endif
#else
	static_assert(false, "AVX or higher required.");
#endif

// definitions of TRUE and FALSE.
#if defined(__INTEL_COMPILER) && defined(SIMD_512)
	#define BOOL bool
	#define TRUE true
	#define FALSE false
#else
	// the GNU compiler seems to want TRUE and FALSE be of the same data type
	// as the numerical values processed in that context (in our case: double).
	// with the Intel compiler this seems to be the better choice too when using AVX(2).
	#define BOOL double
	#define TRUE 1.0
	#define FALSE 0.0
#endif

// simdlen clause for OpenMP 4.x loop vectorization.
#if defined(__INTEL_COMPILER)
	#define SIMDLEN(X) simdlen(X)
#else
	#define SIMDLEN(X)
#endif

#if defined(ENHANCED_EXPLICIT_VECTORIZATION) || defined(MANUAL_VECTORIZATION)
	
	#if !defined(__INTEL_COMPILER)
	// online defined by Intel Compiler
	inline int _mm512_mask2int(__mmask16 k1) {
		return static_cast<int>(k1);
	}
	#endif
	
	// enhanced explicit vectorization: user defined vector data types.
	// we use the logical simdwidth here to allow for vector loop unrolling with OpenMP 4.x.
	typedef struct
	{
		double x[SIMD_WIDTH_LOGICAL_REAL64];
	} vec_real64_t __attribute__((aligned(ALIGNMENT)));

	typedef struct
	{
		BOOL x[SIMD_WIDTH_LOGICAL_REAL64];
	} mask_real64_t __attribute__((aligned(ALIGNMENT)));

	#if defined(SIMD_512)
		#define ALL_LANES_ACTIVE {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE}
	#elif defined(SIMD_256)
		#define ALL_LANES_ACTIVE {TRUE, TRUE, TRUE, TRUE}
	#endif

	// manual vectorization with simd intrinsics: some macro definitions.
	#if defined(SIMD_512)
		#define VEC_REAL64 __m512d
		#define MASK_REAL64 __mmask8
		#define SIMD_TRUE (0x1)
		#define SIMD_FALSE (0x0)
		#define SIMD_SETZERO_REAL64() _mm512_setzero_pd()
		#define SIMD_SET1_REAL64(X1) _mm512_set1_pd(X1)
		#define SIMD_SET1_INT64(X1) _mm512_set1_epi64(X1)
		#define SIMD_SET_MASK_REAL64(M1) _mm512_int2mask(M1)
		#define SIMD_CAST_REAL64_INT64(X1) _mm512_castsi512_pd(X1)
		#define SIMD_CAST_INT64_REAL64(X1) _mm512_castpd_si512(X1)
		#define SIMD_ALL_LANES_ACTIVE_MASK_REAL64 _mm512_int2mask(0xFF)
		#define SIMD_ALL_LANES_ACTIVE_INT (0xFF)
		#define SIMD_ALL_LANES_INACTIVE_INT (0x0)
		#define SIMD_MASK_MOV_REAL64(X0, M1, X1) _mm512_mask_mov_pd(X0, M1, X1)

		#if defined(__MIC__)
			#define SIMD_MASK_STORE_REAL64(ADDR, M1, X1) _mm512_mask_extstore_pd(reinterpret_cast<void*>(ADDR), M1, X1, _MM_DOWNCONV_PD_NONE, _MM_HINT_NONE)
		#else // AVX-512
			// aligned masked store
//			#define SIMD_MASK_STORE_REAL64(ADDR, M1, X1) _mm512_mask_store_pd(reinterpret_cast<void*>(ADDR), M1, X1)
			// unaligned masked store
			#define SIMD_MASK_STORE_REAL64(ADDR, M1, X1) _mm512_mask_storeu_pd(reinterpret_cast<void*>(ADDR), M1, X1)
		#endif

		#define SIMD_CMPGT_REAL64(X1, X2) _mm512_cmp_pd_mask(X1, X2, _CMP_GT_OS)
		#define SIMD_CMPGE_REAL64(X1, X2) _mm512_cmp_pd_mask(X1, X2, _CMP_GE_OS)
		#define SIMD_CMPLT_REAL64(X1, X2) _mm512_cmp_pd_mask(X1, X2, _CMP_LT_OS)
	 	#define SIMD_CHANGE_SIGN_REAL64(X1) _mm512_mul_pd(SIMD_SET1_REAL64(-1.0), X1)
		#define SIMD_MASK_CHANGE_SIGN_REAL64(X0, M1, X1) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_CHANGE_SIGN_REAL64(X1))
		#define SIMD_MUL_REAL64(X1, X2) _mm512_mul_pd(X1, X2)
		#define SIMD_ADD_REAL64(X1, X2) _mm512_add_pd(X1, X2)
		#define SIMD_FMADD_REAL64(X1, X2, X3) _mm512_fmadd_pd(X1, X2, X3)
		#define SIMD_MASK_FMADD_REAL64(X0, M1, X1, X2, X3) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_FMADD_REAL64(X1, X2, X3))
		#define SIMD_SUB_REAL64(X1, X2) _mm512_sub_pd(X1, X2)
		#define SIMD_MASK_MUL_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_MUL_REAL64(X1, X2))
		#define SIMD_MASK_ADD_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_ADD_REAL64(X1, X2))
		#define SIMD_MASK_SUB_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_SUB_REAL64(X1, X2))
		#define SIMD_AND_MASK_REAL64(M1, M2) _mm512_kand(M1, M2)
		#define SIMD_ANDNOT_MASK_REAL64(M1, M2) _mm512_kandn(M1, M2)
		#define SIMD_OR_MASK_REAL64(M1, M2) _mm512_kor(M1, M2)
		#define SIMD_NOT_MASK_REAL64(M1) _mm512_knot(M1)
		#define SIMD_BITMASK_MASK_REAL64(M1) _mm512_mask2int(M1)
		#define SIMD_TEST_ANY_MASK_REAL64(M1) SIMD_BITMASK_MASK_REAL64(M1)
		#define SIMD_AND_INT64(X1, X2) _mm512_and_si512(X1, X2)
		#define SIMD_ABS_REAL64(X1) SIMD_CAST_REAL64_INT64(SIMD_AND_INT64(SIMD_CAST_INT64_REAL64(X1), SIMD_SET1_INT64(0x7FFFFFFFFFFFFFFF)))
		#define SIMD_SQRT_REAL64(X1) _mm512_sqrt_pd(X1)
		#define SIMD_MASK_SQRT_REAL64(X0, M1, X1) _mm512_mask_sqrt_pd(X0, M1, X1)
		#define SIMD_LOG_REAL64(X1) _mm512_log_pd(X1)
		#define SIMD_MASK_LOG_REAL64(X0, M1, X1) _mm512_mask_log_pd(X0, M1, X1)
		#define SIMD_EXP_REAL64(X1) _mm512_exp_pd(X1)
		#define SIMD_MASK_EXP_REAL64(X0, M1, X1) _mm512_mask_exp_pd(X0, M1, X1)
	#elif defined(SIMD_256)
		#define VEC_REAL64 __m256d
		#define MASK_REAL64 __m256d
		#define SIMD_TRUE (0xFFFFFFFFFFFFFFFF)
		#define SIMD_FALSE (0x0)
		#define SIMD_SETZERO_REAL64() _mm256_setzero_pd()
		#define SIMD_SET1_REAL64(X1) _mm256_set1_pd(X1)
		#define SIMD_SET1_INT64(X1) _mm256_set1_epi64x(X1)
		#define SIMD_CAST_REAL64_INT64(X1) _mm256_castsi256_pd(X1)
		#define SIMD_CAST_INT64_REAL64(X1) _mm256_castpd_si256(X1)
		#define SIMD_SET1_MASK_REAL64(X1) SIMD_CAST_REAL64_INT64(SIMD_SET1_INT64(X1))
		#define SIMD_ALL_LANES_ACTIVE_MASK_REAL64 SIMD_SET1_MASK_REAL64(SIMD_TRUE)
		#define SIMD_ALL_LANES_ACTIVE_INT (0xF)
		#define SIMD_ALL_LANES_INACTIVE_INT (0x0)
		#define SIMD_MASK_MOV_REAL64(X0, M1, X1) _mm256_blendv_pd(X0, X1, M1)
		#define SIMD_MASK_STORE_REAL64(ADDR, M1, X1) _mm256_maskstore_pd(reinterpret_cast<double*>(ADDR), SIMD_CAST_INT64_REAL64(M1), X1)
		#define SIMD_CMPGT_REAL64(X1, X2) _mm256_cmp_pd(X1, X2, _CMP_GT_OS)
		#define SIMD_CMPGE_REAL64(X1, X2) _mm256_cmp_pd(X1, X2, _CMP_GE_OS)
		#define SIMD_CMPLT_REAL64(X1, X2) _mm256_cmp_pd(X1, X2, _CMP_LT_OS)
		#define SIMD_CHANGE_SIGN_REAL64(X1) _mm256_mul_pd(SIMD_SET1_REAL64(-1.0), X1)
		#define SIMD_MASK_CHANGE_SIGN_REAL64(X0, M1, X1) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_CHANGE_SIGN_REAL64(X1))
		#define SIMD_MUL_REAL64(X1, X2) _mm256_mul_pd(X1, X2)
		#define SIMD_ADD_REAL64(X1, X2) _mm256_add_pd(X1, X2)
		#if defined(__AVX2__)
			#define SIMD_FMADD_REAL64(X1, X2, X3) _mm256_fmadd_pd(X1, X2, X3)
		#else
			#define SIMD_FMADD_REAL64(X1, X2, X3) SIMD_ADD_REAL64(SIMD_MUL_REAL64(X1, X2), X3)
		#endif
		#define SIMD_SUB_REAL64(X1, X2) _mm256_sub_pd(X1, X2)
		#define SIMD_MASK_MUL_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_MUL_REAL64(X1, X2))
		#define SIMD_MASK_ADD_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_ADD_REAL64(X1, X2))
		#define SIMD_MASK_FMADD_REAL64(X0, M1, X1, X2, X3) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_FMADD_REAL64(X1, X2, X3))
		#define SIMD_MASK_SUB_REAL64(X0, M1, X1, X2) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_SUB_REAL64(X1, X2))
		#define SIMD_AND_MASK_REAL64(M1, M2) _mm256_and_pd(M1, M2)
		#define SIMD_ANDNOT_MASK_REAL64(M1, M2) _mm256_andnot_pd(M1, M2)
		#define SIMD_OR_MASK_REAL64(M1, M2) _mm256_or_pd(M1, M2)
		#define SIMD_NOT_MASK_REAL64(M1) _mm256_xor_pd(M1, SIMD_CAST_REAL64_INT64(SIMD_SET1_INT64(SIMD_TRUE)))
		#define SIMD_BITMASK_MASK_REAL64(M1) _mm256_movemask_pd(M1)
		#define SIMD_TEST_ANY_MASK_REAL64(M1) SIMD_BITMASK_MASK_REAL64(M1)
		#define SIMD_AND_INT64(X1, X2) _mm256_and_si256(X1, X2)
		#define SIMD_ABS_REAL64(X1) SIMD_CAST_REAL64_INT64(SIMD_AND_INT64(SIMD_CAST_INT64_REAL64(X1), SIMD_SET1_INT64(0x7FFFFFFFFFFFFFFF)))
		#define SIMD_SQRT_REAL64(X1) _mm256_sqrt_pd(X1)
		#define SIMD_MASK_SQRT_REAL64(X0, M1, X1) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_SQRT_REAL64(X1))
		#if defined(__INTEL_COMPILER)
			// simd function calls to exp and log via _mm256_[log,exp]_pd is specific to Intel.
			#define SIMD_LOG_REAL64(X1) _mm256_log_pd(X1)
			#define SIMD_MASK_LOG_REAL64(X0, M1, X1) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_LOG_REAL64(X1))
			#define SIMD_EXP_REAL64(X1) _mm256_exp_pd(X1)
			#define SIMD_MASK_EXP_REAL64(X0, M1, X1) SIMD_MASK_MOV_REAL64(X0, M1, SIMD_EXP_REAL64(X1))
		#else
			#define IMPLEMENTATION_REQUIRED
			// GNU compiler has no _mm256_[log,exp]_pd.
			VEC_REAL64 simd_log(const VEC_REAL64& x1);
			VEC_REAL64 simd_mask_log(const VEC_REAL64& x0, const MASK_REAL64& mask, const VEC_REAL64& x1);
			VEC_REAL64 simd_exp(const VEC_REAL64& x1);
			VEC_REAL64 simd_mask_exp(const VEC_REAL64& x0, const MASK_REAL64& mask, const VEC_REAL64& x1);
			#define SIMD_LOG_REAL64(X1) simd_log(X1)
			#define SIMD_MASK_LOG_REAL64(X0, M1, X1) simd_mask_log(X0, M1, X1)
			#define SIMD_EXP_REAL64(X1) simd_exp(X1)
			#define SIMD_MASK_EXP_REAL64(X0, M1, X1) simd_mask_exp(X0, M1, X1)
		#endif
	#endif
#endif

#endif
