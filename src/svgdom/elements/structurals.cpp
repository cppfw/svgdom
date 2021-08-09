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

#include "structurals.hpp"

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

const std::string defs_element::tag = "defs";
const std::string svg_element::tag = "svg";
const std::string mask_element::tag = "mask";
const std::string g_element::tag = "g";
const std::string symbol_element::tag = "symbol";
const std::string use_element::tag = "use";

r4::vector2<real> svg_element::get_dimensions(real dpi) const noexcept{
	real w = this->width.to_px(dpi);
	real h = this->height.to_px(dpi);

	if(w <= 0){
		if(this->view_box[2] > 0){
			w = this->view_box[2];
		}else{
			w = 300;
		}
	}
	
	if(h <= 0){
		if(this->view_box[3] > 0){
			h = this->view_box[3];
		}else{
			h = 150;
		}
	}
	
	return {w, h};
}

real svg_element::aspect_ratio(real dpi)const{
	auto wh = this->get_dimensions(dpi);
	
	if(wh[0] <= 0 || wh[1] <= 0){
		return 0;
	}
	
	return wh[0] / wh[1];
}

void g_element::accept(visitor& v){
	v.visit(*this);
}

void g_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void svg_element::accept(visitor& v){
	v.visit(*this);
}

void svg_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void symbol_element::accept(visitor& v){
	v.visit(*this);
}

void symbol_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void defs_element::accept(visitor& v){
	v.visit(*this);
}

void defs_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void use_element::accept(visitor& v){
	v.visit(*this);
}

void use_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void mask_element::accept(visitor& v){
	v.visit(*this);
}

void mask_element::accept(const_visitor& v) const {
	v.visit(*this);
}
