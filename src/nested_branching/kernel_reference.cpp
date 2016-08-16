// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_reference(const double& x_1, const double& x_2, double& y)
{
	if (x_1 > x_2)
	{
		if (x_2 > 0.2)
			y = x_1 + FUNC_1(x_2);
		else if (x_2 < -0.2)
			y = x_1 - FUNC_1(-1.0 * x_2);
		else
			y = x_1;
	}
	else
	{
		if (x_1 > 0.2)
			y = x_2 + FUNC_2(x_1);
		else if (x_1 < -0.2)
			y = x_2 - FUNC_2(-1.0 * x_1);
		else
			y = x_2;
	}
}
