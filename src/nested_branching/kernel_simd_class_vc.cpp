// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
{
	Vc::double_v temp_y { Vc::One };
	// if (x_1 > x_2)
	Vc::double_m if_mask_l0 { (x_1 > x_2) };
	{
		// if (x_2 > 0.2)
		Vc::double_m if_mask_l1 { (x_2 > 0.2) };
		{
			// y = x_1 + FUNC_1(x_2);
			Vc::double_m temp_mask { if_mask_l0 && if_mask_l1 };
			if (!temp_mask.isEmpty()) // optimization
				temp_y(temp_mask) = x_1 + SIMD_CLASS_FUNC_1(x_2);
		}
		// else if (x_2 < -0.2)
		Vc::double_m else_if_mask_l1 { !if_mask_l1 && (x_2 < -0.2) };
		{
			// y = x_1 - FUNC_1(-1.0 * x_2);
			Vc::double_m temp_mask { if_mask_l0 && else_if_mask_l1 };
			if (!temp_mask.isEmpty()) // optimization
				temp_y(temp_mask) = x_1 - SIMD_CLASS_FUNC_1(-1.0 * x_2);
		}
		// else
		Vc::double_m else_mask_l1 { !if_mask_l1 && !else_if_mask_l1 };
			// y = x_1;
			temp_y(if_mask_l0 && else_mask_l1) = x_1;
	}
	// else
	Vc::double_m else_mask_l0 { !if_mask_l0 };
	{
		// if (x_1 > 0.2)
		Vc::double_m if_mask_l1 { (x_1 > 0.2) };
		{
			// y = x_2 + FUNC_2(x_1);
			Vc::double_m temp_mask { else_mask_l0 && if_mask_l1 };
			if (!temp_mask.isEmpty()) // optimization
				temp_y(temp_mask) = x_2 + SIMD_CLASS_FUNC_2(x_1);
		}
		// else if (x_1 < -0.2)
		Vc::double_m else_if_mask_l1 { (x_1 < -0.2) };
		{
			// y = x_2 - FUNC_2(-1.0 * x_1);
			Vc::double_m temp_mask { else_mask_l0 && else_if_mask_l1 };
			if (!temp_mask.isEmpty()) // optimization
				temp_y(temp_mask) = x_2 - SIMD_CLASS_FUNC_2(-1.0 * x_1);
		}
		// else
		Vc::double_m else_mask_l1 { !if_mask_l1 && !else_if_mask_l1 };
			// y = x_2;
			temp_y(else_mask_l0 && else_mask_l1) = x_2;
	}
	y(mask_0) = temp_y;
}

