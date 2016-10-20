// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

//void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
void kernel_simd_class_umesimd(const UMESIMD_Vec& x_1, const UMESIMD_Vec& x_2, UMESIMD_Vec& y, UMESIMD_Mask& m)
{
	y[m] = SIMD_CLASS_UMESIMD_FUNC_1(0.1 + (x_1 + x_2) * (x_1 + x_2));
}
