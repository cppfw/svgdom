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

#include "gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

const std::string gradient::stop_element::tag = "stop";
const std::string linear_gradient_element::tag = "linearGradient";
const std::string radial_gradient_element::tag = "radialGradient";

void radial_gradient_element::accept(visitor& v){
	v.visit(*this);
}

void radial_gradient_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void linear_gradient_element::accept(visitor& v){
	v.visit(*this);
}

void linear_gradient_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void gradient::stop_element::accept(visitor& v){
	v.visit(*this);
}

void gradient::stop_element::accept(const_visitor& v) const {
	v.visit(*this);
}

std::string gradient::spread_method_to_string() const {
	switch(this->spread_method_){
		default:
		case gradient::spread_method::default_:
			return "";
		case gradient::spread_method::pad:
			return "pad";
		case gradient::spread_method::reflect:
			return "reflect";
		case gradient::spread_method::repeat:
			return "repeat";
	}
}
