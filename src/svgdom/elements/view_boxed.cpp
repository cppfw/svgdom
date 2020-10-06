#include "view_boxed.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;

decltype(view_boxed::view_box) view_boxed::parse_view_box(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(view_boxed::view_box) ret;
	
	for(unsigned i = 0; i != ret.size(); ++i){
		skipWhitespacesAndOrComma(s);
		ret[i] = readInReal(s);
		if(s.fail()){
			return {{-1, -1, -1, -1}};
		}
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
