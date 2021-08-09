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

#include "util.hxx"

#include <utki/string.hpp>

#include <sstream>
#include <cctype>
#include <vector>
#include <charconv>

using namespace svgdom;

std::string svgdom::trim_tail(std::string_view s){
	const auto t = s.find_last_not_of(" \t\n\r");
	if(t == std::string::npos){
		return std::string(s);
	}
	
	return std::string(s.substr(0, t + 1));
}

std::string svgdom::iri_to_local_id(std::string_view iri){
	if(iri.size() != 0 && iri[0] == '#'){
		return std::string(iri.substr(1));
	}
	return std::string();
}

coordinate_units svgdom::parse_coordinate_units(std::string_view s){
	if(s == "userSpaceOnUse"){
		return coordinate_units::user_space_on_use;
	}else if(s == "objectBoundingBox"){
		return coordinate_units::object_bounding_box;
	}
	return coordinate_units::unknown;
}

std::string svgdom::coordinate_units_to_string(coordinate_units u){
	switch(u){
		default:
			return std::string();
		case coordinate_units::object_bounding_box:
			return "objectBoundingBox";
		case coordinate_units::user_space_on_use:
			return "userSpaceOnUse";
	}
}

r4::vector2<real> svgdom::parse_number_and_optional_number(std::string_view s, r4::vector2<real> defaults){
	r4::vector2<real> ret;

	utki::string_parser p(s);

	try{
		ret[0] = p.read_number<real>();
	}catch(std::invalid_argument&){
		return defaults;
	}

	p.skip_whitespaces_and_comma();

	try{
		ret[1] = p.read_number<real>();
	}catch(std::invalid_argument&){
		ret[1] = defaults[1];
	}

	return ret;
}

std::string svgdom::number_and_optional_number_to_string(std::array<real, 2> non, real optional_number_default){
	std::stringstream ss;
	
	ss << non[0];
	
	if(non[1] != optional_number_default){
		ss << " " << non[1];
	}
	
	return ss.str();
}
