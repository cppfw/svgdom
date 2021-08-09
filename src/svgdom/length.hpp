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

#include "config.hpp"

#ifdef IN
#	undef IN
#endif

namespace svgdom{

enum class length_unit{
	unknown,
	number,
	percent,
	em,
	ex,
	px,
	cm,
	in,
	pt,
	pc,
	mm
};

/**
 * @brief SVG attribute value of type 'length'.
 */
struct length{
	real value;
	length_unit unit;
	
	static length parse(std::string_view str);
	
	length() = default;

	constexpr length(real value, length_unit unit = length_unit::number) :
			value(value),
			unit(unit)
	{}

	bool is_valid()const noexcept{
		return this->unit != length_unit::unknown;
	}

	bool is_percent()const noexcept{
		return this->unit == length_unit::percent;
	}
	
	real to_px(real dpi)const noexcept;
	
	bool operator!=(const length& l)const{
		return this->value != l.value || (this->unit != l.unit && this->value != real(0));
	}
};

}

std::ostream& operator<<(std::ostream& s, const svgdom::length& l);
