// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/simd_data_types.hpp"

// function definitions for simd log and exp.
// GNU requires glibc-2.22 or higher (libmvec) to vectorize calls to log and exp within the OpenMP 4.x loops.
// when inlined, these function calls should disappear, leaving only the call to simd log and exp.
#if defined(INTRINSICS) && defined(IMPLEMENTATION_REQUIRED)
VEC_REAL64 simd_log(const VEC_REAL64& x_1)
{
	VEC_REAL64 y;
	double* ptr_y = reinterpret_cast<double*>(&y);
	const double* ptr_x_1 = reinterpret_cast<const double*>(&x_1);
	#pragma omp simd
	for (std::int32_t ii=0; ii<SIMD_WIDTH_NATIVE_REAL64; ii++)
		ptr_y[ii] = std::log(ptr_x_1[ii]);
	return y;
}

VEC_REAL64 simd_mask_log(const VEC_REAL64& x_0, const MASK_REAL64& mask, const VEC_REAL64& x_1)
{
	VEC_REAL64 y = x_0;
	double* ptr_y = reinterpret_cast<double*>(&y);
	const double* ptr_x_1 = reinterpret_cast<const double*>(&x_1);
	const std::uint64_t* ptr_mask = reinterpret_cast<const std::uint64_t*>(&mask);
	#pragma omp simd
	for (std::int32_t ii=0; ii<SIMD_WIDTH_NATIVE_REAL64; ii++)
		if (ptr_mask[ii])
			ptr_y[ii] = std::log(ptr_x_1[ii]);
	return y;
}

VEC_REAL64 simd_exp(const VEC_REAL64& x_1)
{
	VEC_REAL64 y;
	double* ptr_y = reinterpret_cast<double*>(&y);
	const double* ptr_x_1 = reinterpret_cast<const double*>(&x_1);
	#pragma omp simd
	for (std::int32_t ii=0; ii<SIMD_WIDTH_NATIVE_REAL64; ii++)
		ptr_y[ii] = std::exp(ptr_x_1[ii]);
	return y;
}

VEC_REAL64 simd_mask_exp(const VEC_REAL64& x_0, const MASK_REAL64& mask, const VEC_REAL64& x_1)
{
	VEC_REAL64 y = x_0;
	double* ptr_y = reinterpret_cast<double*>(&y);
	const double* ptr_x_1 = reinterpret_cast<const double*>(&x_1);
	const std::uint64_t* ptr_mask = reinterpret_cast<const std::uint64_t*>(&mask);
	#pragma omp simd
	for (std::int32_t ii=0; ii<SIMD_WIDTH_NATIVE_REAL64; ii++)
		if (ptr_mask[ii])
			ptr_y[ii] = std::exp(ptr_x_1[ii]);
	return y;
}
#endif
