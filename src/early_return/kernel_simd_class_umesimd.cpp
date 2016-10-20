// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_umesimd(const UMESIMD_Vec& x_1, const UMESIMD_Vec& x_2, UMESIMD_Vec& y, UMESIMD_Mask& m)
{
	UMESIMD_Mask lanes_active { m }; // set to 0 if lane returns, needs to be used in all assignments.
	// if (x_1 < 0.0)
	UMESIMD_Mask if_mask { x_1 < 0.0 };
		// y = 0.0;
		y[lanes_active && if_mask] = 0.0; // early return lanes, y is set to 0.0
		// return;
		lanes_active = lanes_active && !if_mask; // deactive lanes from if-branch to reflect the return.
	// y = FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2));
	y[lanes_active] = SIMD_CLASS_UMESIMD_FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2)); // perform remaining statement on remaining lanes.
}
