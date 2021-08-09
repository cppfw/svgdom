/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#pragma once

#include "../length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct rectangle{
	length x = length(0, length_unit::unknown);
	length y = length(0, length_unit::unknown);
	length width = length(100, length_unit::unknown);
	length height = length(100, length_unit::unknown);
	
	rectangle(){}
	
	constexpr rectangle(length x, length y, length width, length height) :
			x(x), y(y), width(width), height(height)
	{}
	
	bool is_x_specified()const noexcept{
		return this->x.unit != length_unit::unknown;
	}

	bool is_y_specified()const noexcept{
		return this->y.unit != length_unit::unknown;
	}
	
	bool is_width_specified()const noexcept{
		return this->width.unit != length_unit::unknown;
	}
	
	bool is_height_specified()const noexcept{
		return this->height.unit != length_unit::unknown;
	}
};

}
