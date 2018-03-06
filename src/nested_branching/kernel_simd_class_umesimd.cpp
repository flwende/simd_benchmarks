// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_simd_class_umesimd(const UMESIMD_Vec& x_1, const UMESIMD_Vec& x_2, UMESIMD_Vec& y, UMESIMD_Mask& m)
{
    UMESIMD_Mask m0 = x_1 > x_2;
    UMESIMD_Mask m1 = x_2 > 0.2;
    UMESIMD_Mask m2 = x_2 < -0.2;
    UMESIMD_Mask m3 = x_1 > 0.2;
    UMESIMD_Mask m4 = x_1 < -0.2;

    // initialize with default ('else-else') value
    y[m] = x_2;
    // if (x_1 > x_2)
        // else
        y[m && m0] = x_1;
        // if (x_2 > 0.2)
        y[m && m0 && m1] = x_1 + SIMD_CLASS_UMESIMD_FUNC_1(x_2);
        // else if (x_2 < -0.2)
        y[m && m0 && !m1 && m2] = x_1 - SIMD_CLASS_UMESIMD_FUNC_1(-1.0 * x_2);
    
    // else
        // if (x_1 > 0.2)
        y[m && !m0 && m3] = x_2 + SIMD_CLASS_UMESIMD_FUNC_2(x_1);
        // else if (x_1 < -0.2)
        y[m && !m0 && !m3 && m4] = x_2 - SIMD_CLASS_UMESIMD_FUNC_2(-1.0 * x_1);
}
