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

#include "length.hpp"

#include <cmath>
#include <string_view>

#include "util.hxx"

using namespace svgdom;

length length::parse(std::string_view str){
	length ret;

	utki::string_parser p(str);

	ret.value = p.read_number<real>();

	auto unit = p.read_word();

	if(unit.empty()){
		ret.unit = length_unit::number;
	}else if(unit == "%"){
		ret.unit = length_unit::percent;
	}else if(unit == "em"){
		ret.unit = length_unit::em;
	}else if(unit == "ex"){
		ret.unit = length_unit::ex;
	}else if(unit == "px"){
		ret.unit = length_unit::px;
	}else if(unit == "cm"){
		ret.unit = length_unit::cm;
	}else if(unit == "mm"){
		ret.unit = length_unit::mm;
	}else if(unit == "in"){
		ret.unit = length_unit::in;
	}else if(unit == "pt"){
		ret.unit = length_unit::pt;
	}else if(unit == "pc"){
		ret.unit = length_unit::pc;
	}else{
		ret.unit = length_unit::unknown;
	}
	
	return ret;
}

real length::to_px(real dpi) const noexcept{
	switch(this->unit){
		default:
			return 0;
		case svgdom::length_unit::number:
		case svgdom::length_unit::px:
			return this->value;
		case svgdom::length_unit::in:
			return std::ceil(this->value * dpi);
		case svgdom::length_unit::cm:
			return std::ceil(this->value * (dpi / real(2.54)));
		case svgdom::length_unit::mm:
			return std::ceil(this->value * (dpi / real(25.4)));
		case svgdom::length_unit::pt: // 1 pt = 1/72 of an inch
			return std::ceil(this->value * (dpi / real(72)));
		case svgdom::length_unit::pc: // 1 pc = 1/6 of an inch
			return std::ceil(this->value * (dpi / real(6)));
		case svgdom::length_unit::em:
		case svgdom::length_unit::ex:
			// em and ex depend on the font size. Text is not supported by svgdom, so return 0 size.
			return 0;
	}
}

std::ostream& operator<<(std::ostream& s, const length& l){
	s << l.value;
	
	switch(l.unit){
		case length_unit::unknown:
		case length_unit::number:
		default:
			break;
		case length_unit::percent:
			s << "%";
			break;
		case length_unit::em:
			s << "em";
			break;
		case length_unit::ex:
			s << "ex";
			break;
		case length_unit::px:
			s << "px";
			break;
		case length_unit::cm:
			s << "cm";
			break;
		case length_unit::mm:
			s << "mm";
			break;
		case length_unit::in:
			s << "in";
			break;
		case length_unit::pt:
			s << "pt";
			break;
		case length_unit::pc:
			s << "pc";
			break;
	}
	
	return s;
}
