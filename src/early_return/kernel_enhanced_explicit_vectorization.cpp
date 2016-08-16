// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_enhanced_explicit_vectorization(const vec_real64_t& x_1, const vec_real64_t& x_2, vec_real64_t& y, const mask_real64_t& mask_0)
{
	#pragma omp simd SIMDLEN(SIMD_WIDTH_LOGICAL_REAL64)
	for (std::int32_t ii = 0; ii < SIMD_WIDTH_LOGICAL_REAL64; ++ii)
	{
		if (mask_0.x[ii])
		{
			const double temp_x_1 = x_1.x[ii];
			const double temp_x_2 = x_2.x[ii];
			if (temp_x_1 < 0.0)
				y.x[ii] = 0.0;
			else
				y.x[ii] = FUNC_1(0.1 + (temp_x_1 + temp_x_2) * (temp_x_1 + temp_x_2));
		}
	}
}
