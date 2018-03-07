// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

// maximum number of while loop iterations.
#define Y_STOP 10.0

void kernel_enhanced_explicit_vectorization(const vec_real64_t& x_1, const vec_real64_t& x_2, vec_real64_t& y, const mask_real64_t& mask_0)
{
	mask_real64_t lane_alife;
	#pragma	omp simd SIMDLEN(SIMD_WIDTH_LOGICAL_REAL64)
	for (std::int32_t ii = 0; ii < SIMD_WIDTH_LOGICAL_REAL64; ++ii)
	{
		const BOOL temp_mask_0 = mask_0.x[ii];
		if (temp_mask_0)
			y.x[ii] = 0.0;
		lane_alife.x[ii] = temp_mask_0;
	}
	std::int32_t continue_loop = 1;
	while (continue_loop)
	{
		continue_loop = 0;
		#pragma omp simd SIMDLEN(SIMD_WIDTH_LOGICAL_REAL64) reduction(+ : continue_loop)
		for (std::int32_t ii = 0; ii < SIMD_WIDTH_LOGICAL_REAL64; ++ii)
		{
			double temp_y = y.x[ii];
			const double temp_x_1 = x_1.x[ii];
			const double temp_x_2 = x_2.x[ii];
			const BOOL temp_lane_alife = lane_alife.x[ii];
			if (temp_y < 0.0)
				temp_y *= -1.0;
			if (temp_lane_alife && (temp_x_1 + temp_x_2) < 0.0)
				temp_y += FUNC_1(0.1 + temp_y - (temp_x_1 + temp_x_2));
			else if (temp_lane_alife)
				temp_y += FUNC_2(0.1 + temp_y + (temp_x_1 + temp_x_2));
			if (temp_lane_alife)
			{
				y.x[ii] = temp_y;
				if (temp_y >= Y_STOP)
					lane_alife.x[ii] = FALSE;
				else
					continue_loop += 1;
			}
		}
	}
}
