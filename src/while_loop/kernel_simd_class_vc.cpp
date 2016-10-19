// Copyright (c) 2016 Matthias Noack (ma.noack.pr@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include "common/kernel.hpp"

// maximum number of while loop iterations.
#define Y_STOP 10.0

void kernel_simd_class_vc(const Vc::double_v& x_1, const Vc::double_v& x_2, Vc::double_v& y, Vc::double_m& mask_0)
{
	Vc::double_v temp_y { Vc::Zero };
	Vc::double_v x_1_plus_x_2 { x_1 + x_2 };
	Vc::double_m lanes_active { mask_0 }; // mask for active simd lanes inside the while loop.
	// while (temp_y < Y_STOP)
	while (!(lanes_active = lanes_active && (temp_y < Y_STOP)).isEmpty()) // update active lanes with loop condition and update lanes_active for use inside loop body
	{
		Vc::double_v temp_y_w { temp_y }; // inner temp to avoid some lanes_active mask operations.
		//if (temp_y_w < 0.0) temp_y_w *= -1.0;
		temp_y_w((temp_y_w < 0.0)) *= -1.0;
		Vc::double_v const_plus_y = 0.1 + temp_y_w;
		// if ((x_1 + x_2) < 0.0)
		Vc::double_m if_mask { x_1_plus_x_2 < 0.0 }; // if condition and active lane.
		// temp_y_w += FUNC_1(0.1 + temp_y_w - (x_1 + x_2));
		if(!if_mask.isEmpty()) // optimization
			temp_y_w(if_mask) += SIMD_CLASS_FUNC_1(const_plus_y - x_1_plus_x_2); // if body
		// else
		// temp_y_w += FUNC_2(0.1 + temp_y_w + (x_1 + x_2));
		if(!(!if_mask).isEmpty()) // optimization
			temp_y_w(!if_mask) += SIMD_CLASS_FUNC_2(const_plus_y + x_1_plus_x_2); // else body
		temp_y(lanes_active) = temp_y_w;
	}
	y(mask_0) = temp_y;	
}
