// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

//  maximum number of while loop iterations.
#define Y_STOP 10.0

#pragma omp declare simd		   \
	simdlen(SIMD_WIDTH_LOGICAL_REAL64) \
	linear(x_1 : 1)			   \
	linear(x_2 : 1)			   \
	linear(y : 1)			   \
	aligned(x_1 : ALIGNMENT)	   \
	aligned(x_2 : ALIGNMENT)	   \
	aligned(y : ALIGNMENT)
void kernel_explicit_vectorization(const double*__restrict__ x_1, const double*__restrict__ x_2, double*__restrict__ y)
{
	double temp_y = 0.0;
	while (temp_y < Y_STOP)
	{
		if (temp_y < 0.0)
			temp_y *= -1.0;
		if (((*x_1) + (*x_2)) < 0.0)
			temp_y += FUNC_1(0.1 + temp_y - ((*x_1) + (*x_2)));
		else
			temp_y += FUNC_2(0.1 + temp_y + ((*x_1) + (*x_2)));
	}
	(*y) = temp_y;
}
