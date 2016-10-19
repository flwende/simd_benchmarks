// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
{
	y(mask_0) = SIMD_CLASS_FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2));
}
