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

#include "filter.hpp"
#include "../visitor.hpp"

using namespace svgdom;

const std::string filter_element::tag = "filter";
const std::string fe_gaussian_blur_element::tag = "feGaussianBlur";
const std::string fe_color_matrix_element::tag = "feColorMatrix";
const std::string fe_blend_element::tag = "feBlend";
const std::string fe_composite_element::tag = "feComposite";

void filter_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void filter_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_blend_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_composite_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_blend_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_composite_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

r4::vector2<real> fe_gaussian_blur_element::get_std_deviation()const noexcept{
	if(!this->is_std_deviation_specified()){
		return 0;
	}
	
	if(this->is_std_deviation_y_specified()){
		return this->std_deviation;
	}
	
	return this->std_deviation.x();
}
