#include "ViewBoxed.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;

decltype(ViewBoxed::viewBox) ViewBoxed::parseViewbox(const std::string& str) {
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(ViewBoxed::viewBox) ret;
	
	for(unsigned i = 0; i != ret.size(); ++i){
		ret[i] = readInReal(s);
		if(s.fail()){
			return {{-1, -1, -1, -1}};
		}
	}
	
	return ret;
}

std::string ViewBoxed::viewBoxToString() const {
	std::stringstream s;
	bool isFirst = true;
	for (auto i = this->viewBox.begin(); i != this->viewBox.end(); ++i) {
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
