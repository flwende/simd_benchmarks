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
	if (bitmask == SIMD_ALL_LANES_ACTIVE_INT)
	{
		MASK_REAL64 m_2 = SIMD_CMPGT_REAL64(x_1, x_2);
		VEC_REAL64 temp_y = SIMD_MASK_MOV_REAL64(x_2, m_2, x_1);
		MASK_REAL64 m_3 = SIMD_AND_MASK_REAL64(m_2, SIMD_CMPGT_REAL64(x_2, SIMD_SET1_REAL64(0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_3))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_1(temp_1, m_3, x_2);
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_3, x_2));
			#endif
			temp_y = SIMD_MASK_ADD_REAL64(temp_y, m_3, x_1, temp_1);
		}
		MASK_REAL64 m_4 = SIMD_AND_MASK_REAL64(m_2, SIMD_CMPLT_REAL64(x_2, SIMD_SET1_REAL64(-0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_4))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_1(temp_1, m_4, SIMD_CHANGE_SIGN_REAL64(x_2));
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_4, SIMD_CHANGE_SIGN_REAL64(x_2)));
			#endif
			temp_y = SIMD_MASK_SUB_REAL64(temp_y, m_4, x_1, temp_1);
		}
		MASK_REAL64 m_5 = SIMD_ANDNOT_MASK_REAL64(m_2, SIMD_CMPGT_REAL64(x_1, SIMD_SET1_REAL64(0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_5))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_2(temp_1, m_5, x_1);
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_5, x_1));
			#endif
			temp_y = SIMD_MASK_ADD_REAL64(temp_y, m_5, x_2, temp_1);
		}
		MASK_REAL64 m_6 = SIMD_ANDNOT_MASK_REAL64(m_2, SIMD_CMPLT_REAL64(x_1, SIMD_SET1_REAL64(-0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_6))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_2(temp_1, m_6, SIMD_CHANGE_SIGN_REAL64(x_1));
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_6, SIMD_CHANGE_SIGN_REAL64(x_1)));
			#endif
			temp_y = SIMD_MASK_SUB_REAL64(temp_y, m_6, x_2, temp_1);
		}
		y = temp_y;
	}
	else
	{
		MASK_REAL64 m_2 = SIMD_CMPGT_REAL64(x_1, x_2);
		VEC_REAL64 temp_y = SIMD_MASK_MOV_REAL64(x_2, m_2, x_1);
		MASK_REAL64 m_3 = SIMD_AND_MASK_REAL64(m_2, SIMD_CMPGT_REAL64(x_2, SIMD_SET1_REAL64(0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_3))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_1(temp_1, m_3, x_2);
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_3, x_2));
			#endif
			temp_y = SIMD_MASK_ADD_REAL64(temp_y, m_3, x_1, temp_1);
		}
		MASK_REAL64 m_4 = SIMD_AND_MASK_REAL64(m_2, SIMD_CMPLT_REAL64(x_2, SIMD_SET1_REAL64(-0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_4))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_1(temp_1, m_4, SIMD_CHANGE_SIGN_REAL64(x_2));
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_1(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_4, SIMD_CHANGE_SIGN_REAL64(x_2)));
			#endif
			temp_y = SIMD_MASK_SUB_REAL64(temp_y, m_4, x_1, temp_1);
		}
		MASK_REAL64 m_5 = SIMD_ANDNOT_MASK_REAL64(m_2, SIMD_CMPGT_REAL64(x_1, SIMD_SET1_REAL64(0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_5))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_2(temp_1, m_5, x_1);
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_5, x_1));
			#endif
			temp_y = SIMD_MASK_ADD_REAL64(temp_y, m_5, x_2, temp_1);
		}
		MASK_REAL64 m_6 = SIMD_ANDNOT_MASK_REAL64(m_2, SIMD_CMPLT_REAL64(x_1, SIMD_SET1_REAL64(-0.2)));
		if (SIMD_TEST_ANY_MASK_REAL64(m_6))
		{
			#if defined(SIMD_512)
				VEC_REAL64 temp_1 = SIMD_MASK_FUNC_2(temp_1, m_6, SIMD_CHANGE_SIGN_REAL64(x_1));
			#else
				VEC_REAL64 temp_1 = SIMD_FUNC_2(SIMD_MASK_MOV_REAL64(SIMD_SET1_REAL64(1.0), m_6, SIMD_CHANGE_SIGN_REAL64(x_1)));
			#endif
			temp_y = SIMD_MASK_SUB_REAL64(temp_y, m_6, x_2, temp_1);
		}
		SIMD_MASK_STORE_REAL64(&y, mask_0, temp_y);
	}
}
