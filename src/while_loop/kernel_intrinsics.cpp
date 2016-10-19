// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

#if !defined(__INTEL_COMPILER)
	#include "../../src/common/simd_data_types.cpp"
#endif

// maximum number of while loop iterations.
#define Y_STOP 10.0

void kernel_intrinsics(const VEC_REAL64& x_1, const VEC_REAL64& x_2, VEC_REAL64& y, const MASK_REAL64& mask_0)
{
	MASK_REAL64 lane_alife = mask_0;
	bool all_lanes_alife = (SIMD_BITMASK_MASK_REAL64(lane_alife) == SIMD_ALL_LANES_ACTIVE_INT);
	VEC_REAL64 temp_y = SIMD_SETZERO_REAL64();
	while (SIMD_TEST_ANY_MASK_REAL64(lane_alife))
	{
		MASK_REAL64 m_2 = SIMD_CMPLT_REAL64(temp_y, SIMD_SETZERO_REAL64());
		temp_y = SIMD_MASK_CHANGE_SIGN_REAL64(temp_y, m_2, temp_y);
		// unconditional execution if all lanes are active.
		if (all_lanes_alife)
		{
			MASK_REAL64 m_3 = SIMD_CMPLT_REAL64(SIMD_ADD_REAL64(x_1, x_2), SIMD_SETZERO_REAL64());
			MASK_REAL64 m_4 = SIMD_NOT_MASK_REAL64(m_3);
			VEC_REAL64 temp_1;
			#if defined(SIMD_512)
				if (SIMD_TEST_ANY_MASK_REAL64(m_3))
				{
					VEC_REAL64 temp_2 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_SUB_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
					temp_1 = SIMD_ADD_REAL64(temp_y, SIMD_MASK_FUNC_1(temp_1, m_3, temp_2));
				}
				if (SIMD_TEST_ANY_MASK_REAL64(m_4))
					{
						VEC_REAL64 temp_3 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_ADD_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_MASK_ADD_REAL64(temp_1, m_4, temp_y, SIMD_MASK_FUNC_2(temp_3, m_4, temp_3));
					}
			#else
				if (SIMD_TEST_ANY_MASK_REAL64(m_3))
					{
						VEC_REAL64 temp_2 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_SUB_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_ADD_REAL64(temp_y, SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_3, temp_2)));
					}
				if (SIMD_TEST_ANY_MASK_REAL64(m_4))
					{
						VEC_REAL64 temp_3 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_ADD_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_MASK_ADD_REAL64(temp_1, m_4, temp_y, SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_4, temp_3)));
					}
			#endif
			temp_y = temp_1;
			lane_alife = SIMD_AND_MASK_REAL64(lane_alife, SIMD_CMPLT_REAL64(temp_y, SIMD_SET1_REAL64(Y_STOP)));
			all_lanes_alife = SIMD_BITMASK_MASK_REAL64(lane_alife) == SIMD_ALL_LANES_ACTIVE_INT;
		}
		else
		{
  		        MASK_REAL64 m_2 = SIMD_CMPLT_REAL64(SIMD_ADD_REAL64(x_1, x_2), SIMD_SETZERO_REAL64());
			MASK_REAL64 m_3 = SIMD_AND_MASK_REAL64(m_2, lane_alife);
			MASK_REAL64 m_4 = SIMD_ANDNOT_MASK_REAL64(m_2, lane_alife);
			VEC_REAL64 temp_1;
			#if defined(SIMD_512)
				if (SIMD_TEST_ANY_MASK_REAL64(m_3))
					{
						VEC_REAL64 temp_2 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_SUB_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_ADD_REAL64(temp_y, SIMD_MASK_FUNC_1(temp_1, m_3, temp_2));
					}
				if (SIMD_TEST_ANY_MASK_REAL64(m_4))
					{
						VEC_REAL64 temp_3 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_ADD_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_MASK_ADD_REAL64(temp_1, m_4, temp_y, SIMD_MASK_FUNC_2(temp_3, m_4, temp_3));
					}
			#else
				if (SIMD_TEST_ANY_MASK_REAL64(m_3))
					{
						VEC_REAL64 temp_2 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_SUB_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
						temp_1 = SIMD_ADD_REAL64(temp_y, SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_3, temp_2)));
					}
			if (SIMD_TEST_ANY_MASK_REAL64(m_4))
				{
					VEC_REAL64 temp_3 = SIMD_ADD_REAL64(SIMD_SET1_REAL64(0.1), SIMD_ADD_REAL64(temp_y, SIMD_ADD_REAL64(x_1, x_2)));
					temp_1 = SIMD_MASK_ADD_REAL64(temp_1, m_4, temp_y, SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_4, temp_3)));
				}
			#endif
			temp_y = SIMD_MASK_MOV_REAL64(temp_y, SIMD_OR_MASK_REAL64(m_3, m_4), temp_1);
			lane_alife = SIMD_AND_MASK_REAL64(lane_alife, SIMD_CMPLT_REAL64(temp_y, SIMD_SET1_REAL64(Y_STOP)));
		}
	}
	// use unconditional store in case of all lanes have been active initially.
	if (SIMD_BITMASK_MASK_REAL64(mask_0) == SIMD_ALL_LANES_ACTIVE_INT)
		y = temp_y;
	else
		SIMD_MASK_STORE_REAL64(&y, mask_0, temp_y);
}
