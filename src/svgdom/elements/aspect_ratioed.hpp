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

#include <string>

namespace svgdom{
struct aspect_ratioed{
	enum class aspect_ratio_preservation{
		none,
		x_min_y_min,
		x_mid_y_min,
		x_max_y_min,
		x_min_y_mid,
		x_mid_y_mid,
		x_max_y_mid,
		x_min_y_max,
		x_mid_y_max,
		x_max_y_max
	};

	struct aspect_ratio_preservation_value{
		aspect_ratio_preservation preserve = aspect_ratio_preservation::none;
		bool defer = false;
		bool slice = false;

		std::string to_string()const;
		void parse(std::string_view str);
	} preserve_aspect_ratio;
};

}
