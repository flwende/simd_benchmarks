// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
{
	Vc::double_m lanes_active { mask_0 }; // set to 0 if lane returns, needs to be used in all assignments.
	// if (x_1 < 0.0)
	Vc::double_m if_mask { x_1 < 0.0 };
		// y = 0.0;
		y(lanes_active && if_mask) = 0.0; // early return lanes, y is set to 0.0
		// return;
		lanes_active = lanes_active && !if_mask; // deactive lanes from if-branch to reflect the return.
	// y = FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2));
	y(lanes_active) = SIMD_CLASS_FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2)); // perform remaining statement on remaining lanes.
}
