// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_vectorization(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
{
	Vc::double_v temp_y { Vc::One };
	// if (x_1 > x_2 && x_2 >= 0.0)
	Vc::double_m if_mask { (x_1 > x_2 && x_2 >= 0.0) };
		// y = FUNC_1(x_1 + x_2);
		temp_y(if_mask) = SIMD_CLASS_FUNC_1(x_1 + x_2);
	// else
		// y = 1.0; assigned above already
	y(mask_0) = temp_y;
}
