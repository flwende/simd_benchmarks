// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_umesimd(const UMESIMD_Vec& x_1, const UMESIMD_Vec& x_2, UMESIMD_Vec& y, UMESIMD_Mask& m)
{
	UMESIMD_Vec temp_y { 1.0 };
	// if (x_1 > x_2 && x_2 >= 0.0)
	UMESIMD_Mask if_mask { (x_1 > x_2 && x_2 >= 0.0) };
		// y = FUNC_1(x_1 + x_2);
		temp_y[if_mask] = SIMD_CLASS_UMESIMD_FUNC_1(x_1 + x_2);
	// else
		// y = 1.0; assigned above already
	y[m] = temp_y;
}
