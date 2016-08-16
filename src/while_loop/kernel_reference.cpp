// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

// maximum number of while loop iterations.
#define Y_STOP 10.0

void kernel_reference(const double& x_1, const double& x_2, double& y)
{
	double temp_y = 0.0;
	while (temp_y < Y_STOP)
	{
		if (temp_y < 0.0)
			temp_y *= -1.0;
		if ((x_1 + x_2) < 0.0)
			temp_y += FUNC_1(0.1 + temp_y - (x_1 + x_2));
		else
			temp_y += FUNC_2(0.1 + temp_y + (x_1 + x_2));
	}
	y = temp_y;
}
