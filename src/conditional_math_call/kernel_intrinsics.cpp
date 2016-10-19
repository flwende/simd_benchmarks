// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

#if !defined(__INTEL_COMPILER)
	#include "../../src/common/simd_data_types.cpp"
#endif

void kernel_intrinsics(const VEC_REAL64& x_1, const VEC_REAL64& x_2, VEC_REAL64& y, const MASK_REAL64& mask_0)
{
	std::int32_t bitmask = SIMD_BITMASK_MASK_REAL64(mask_0);
	// if all lanes are inactive return immediately.
	if (bitmask == SIMD_ALL_LANES_INACTIVE_INT)
	{
		return;
	}
	// if all lanes are active execute unconditionally.
	else if (bitmask == SIMD_ALL_LANES_ACTIVE_INT)
	{
		MASK_REAL64 mask_1 = SIMD_AND_MASK_REAL64(SIMD_CMPGT_REAL64(x_1, x_2), SIMD_CMPGE_REAL64(x_2, SIMD_SETZERO_REAL64()));
		VEC_REAL64 temp_y;
		if (SIMD_TEST_ANY_MASK_REAL64(mask_1))
			#if defined(SIMD_512)
				temp_y = SIMD_MASK_FUNC_1(temp_y, mask_1, SIMD_ADD_REAL64(x_1, x_2));
			#else
				temp_y = SIMD_FUNC_1(SIMD_MASK_ADD_REAL64(SIMD_SET1_REAL64(1.0), mask_1, x_1, x_2));
			#endif
		y = SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), mask_1, temp_y);
	}
	else
	{
		MASK_REAL64 mask_1 = SIMD_AND_MASK_REAL64(mask_0, SIMD_AND_MASK_REAL64(SIMD_CMPGT_REAL64(x_1, x_2), SIMD_CMPGE_REAL64(x_2, SIMD_SETZERO_REAL64())));
		VEC_REAL64 temp_y;
		if (SIMD_TEST_ANY_MASK_REAL64(mask_1))
			#if defined(SIMD_512)
				temp_y = SIMD_MASK_FUNC_1(temp_y, mask_1, SIMD_ADD_REAL64(x_1, x_2));
			#else
				temp_y = SIMD_FUNC_1(SIMD_MASK_ADD_REAL64(SIMD_SET1_REAL64(1.0), mask_1, x_1, x_2));
			#endif
		temp_y = SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), mask_1, temp_y);
		SIMD_MASK_STORE_REAL64(&y, mask_0, temp_y);
	}
}
