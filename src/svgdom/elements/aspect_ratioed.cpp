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

#include "aspect_ratioed.hpp"

#include <sstream>

#include "../util.hxx"

using namespace svgdom;

namespace{
aspect_ratioed::aspect_ratio_preservation string_to_preserve_aspect_ratio(std::string_view str){
	if(str == "none"){
		return aspect_ratioed::aspect_ratio_preservation::none;
	}else if(str == "xMinYMin"){
		return aspect_ratioed::aspect_ratio_preservation::x_min_y_min;
	}else if(str == "xMidYMin"){
		return aspect_ratioed::aspect_ratio_preservation::x_mid_y_min;
	}else if(str == "xMaxYMin"){
		return aspect_ratioed::aspect_ratio_preservation::x_max_y_min;
	}else if(str == "xMinYMid"){
		return aspect_ratioed::aspect_ratio_preservation::x_min_y_mid;
	}else if(str == "xMidYMid"){
		return aspect_ratioed::aspect_ratio_preservation::x_mid_y_mid;
	}else if(str == "xMaxYMid"){
		return aspect_ratioed::aspect_ratio_preservation::x_max_y_mid;
	}else if(str == "xMinYMax"){
		return aspect_ratioed::aspect_ratio_preservation::x_min_y_max;
	}else if(str == "xMidYMax"){
		return aspect_ratioed::aspect_ratio_preservation::x_mid_y_max;
	}else if(str == "xMaxYMax"){
		return aspect_ratioed::aspect_ratio_preservation::x_max_y_max;
	}
	return aspect_ratioed::aspect_ratio_preservation::none;
}
}

void aspect_ratioed::aspect_ratio_preservation_value::parse(std::string_view s){
	utki::string_parser p(s);

	p.skip_whitespaces();

	auto w = p.read_word();

	p.skip_whitespaces();
	
	if(w == "defer"){
		this->defer = true;
		w = p.read_word();
		if(w.empty()){
			return;
		}
	}else{
		this->defer = false;
	}

	this->preserve = string_to_preserve_aspect_ratio(w);

	w = p.read_word();
	if(w.empty()){
		return;
	}

	if(w == "meet"){
		this->slice = false;
	}else if(w == "slice"){
		this->slice = true;
	}
}

std::string aspect_ratioed::aspect_ratio_preservation_value::to_string()const{
	std::stringstream s;
	if (this->defer) {
		s << "defer ";
	}

	switch(this->preserve){
		default:
			break;
		case aspect_ratioed::aspect_ratio_preservation::none:
			s << "none";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_min_y_min:
			s << "xMinYMin";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_mid_y_min:
			s << "xMidYMin";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_max_y_min:
			s << "xMaxYMin";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_min_y_mid:
			s << "xMinYMid";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_mid_y_mid:
			s << "xMidYMid";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_max_y_mid:
			s << "xMaxYMid";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_min_y_max:
			s << "xMinYMax";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_mid_y_max:
			s << "xMidYMax";
			break;
		case aspect_ratioed::aspect_ratio_preservation::x_max_y_max:
			s << "xMaxYMax";
			break;
	}

	if (this->slice) {
		s << " slice";
	}
	return s.str();
}
