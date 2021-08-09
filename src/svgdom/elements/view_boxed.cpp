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

#include "view_boxed.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;

decltype(view_boxed::view_box) view_boxed::parse_view_box(std::string_view str){
	decltype(view_boxed::view_box) ret;
	
	try{
		utki::string_parser p(str);

		for(unsigned i = 0; i != ret.size(); ++i){
			p.skip_whitespaces_and_comma();
			ret[i] = p.read_number<real>();
		}
	}catch(std::invalid_argument&){
		return {{-1, -1, -1, -1}};
	}
	
	return ret;
}

std::string view_boxed::view_box_to_string()const{
	std::stringstream s;
	bool isFirst = true;
	for (auto i = this->view_box.begin(); i != this->view_box.end(); ++i) {
		if (isFirst) {
			isFirst = false;
		}
		else {
			s << " ";
		}
		s << (*i);
	}
	return s.str();
}
