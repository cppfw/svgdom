#include "ViewBoxed.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;


namespace{

ViewBoxed::PreserveAspectRatio_e stringToPreserveAspectRatio(const std::string& str){
	if(str == "none"){
		return ViewBoxed::PreserveAspectRatio_e::NONE;
	}else if(str == "xMinYMin"){
		return ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MIN;
	}else if(str == "xMidYMin"){
		return ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MIN;
	}else if(str == "xMaxYMin"){
		return ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MIN;
	}else if(str == "xMinYMid"){
		return ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MID;
	}else if(str == "xMidYMid"){
		return ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MID;
	}else if(str == "xMaxYMid"){
		return ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MID;
	}else if(str == "xMinYMax"){
		return ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MAX;
	}else if(str == "xMidYMax"){
		return ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MAX;
	}else if(str == "xMaxYMax"){
		return ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MAX;
	}
	return ViewBoxed::PreserveAspectRatio_e::NONE;
}
}


void ViewBoxed::parseAndFillPreserveAspectRatio(const std::string& str) {
	std::istringstream s(str);
	
	s >> std::skipws;
	
	std::string tmp;
	
	s >> tmp;
	
	if(s.fail()){
		return;
	}
	
	if(tmp == "defer"){
		this->preserveAspectRatio.defer = true;
		s >> tmp;
		if(s.fail()){
			return;
		}
	}else{
		this->preserveAspectRatio.defer = false;
	}
	
	this->preserveAspectRatio.preserve = stringToPreserveAspectRatio(tmp);
	
	s >> tmp;
	if(s.fail()){
		return;
	}
	
	if(tmp == "meet"){
		this->preserveAspectRatio.slice = false;
	}else if(tmp == "slice"){
		this->preserveAspectRatio.slice = true;
	}
}

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

std::string ViewBoxed::preserveAspectRatioToString() const {
	std::stringstream s;
	if (this->preserveAspectRatio.defer) {
		s << "defer ";
	}

switch(this->preserveAspectRatio.preserve){
		default:
			break;
		case ViewBoxed::PreserveAspectRatio_e::NONE:
			s << "none";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MIN:
			s << "xMinYMin";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MIN:
			s << "xMidYMin";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MIN:
			s << "xMaxYMin";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MID:
			s << "xMinYMid";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MID:
			s << "xMidYMid";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MID:
			s << "xMaxYMid";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MIN_Y_MAX:
			s << "xMinYMax";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MID_Y_MAX:
			s << "xMidYMax";
			break;
		case ViewBoxed::PreserveAspectRatio_e::X_MAX_Y_MAX:
			s << "xMaxYMax";
			break;
	}

	if (this->preserveAspectRatio.slice) {
		s << " slice";
	}
	return s.str();
}
