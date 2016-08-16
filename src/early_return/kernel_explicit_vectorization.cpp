// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

#if !defined(__CLANG_COMPILER__)
	#pragma omp declare simd \
	simdlen(SIMD_WIDTH_LOGICAL_REAL64) \
	linear(x_1 : 1) \
	linear(x_2 : 1) \
	linear(y : 1) \
	aligned(x_1 : ALIGNMENT) \
	aligned(x_2 : ALIGNMENT) \
	aligned(y : ALIGNMENT)
#endif
void kernel_explicit_vectorization(const double*__restrict__ x_1, const double*__restrict__ x_2, double*__restrict__ y)
{
	if ((*x_1) < 0.0)
	{
		(*y) = 0.0;
		return;
	}
	(*y) = FUNC_1(0.1 + ((*x_1) + (*x_2)) * ((*x_1) + (*x_2)));
}
