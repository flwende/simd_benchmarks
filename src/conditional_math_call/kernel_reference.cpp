// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

void kernel_reference(const double& x_1, const double& x_2, double& y)
{
	if (x_1 > x_2 && x_2 >= 0.0)
		y = FUNC_1(x_1 + x_2);
	else
		y = 1.0;
}
