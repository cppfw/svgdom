#include "ViewBoxed.hpp"

#include <utki/debug.hpp>

using namespace svgdom;


namespace{

PreserveAspectRatio_e stringToPreserveAspectRatio(const std::string& str){
	if(str == "none"){
		return PreserveAspectRatio_e::NONE;
	}else if(str == "xMinYMin"){
		return PreserveAspectRatio_e::X_MIN_Y_MIN;
	}else if(str == "xMidYMin"){
		return PreserveAspectRatio_e::X_MID_Y_MIN;
	}else if(str == "xMaxYMin"){
		return PreserveAspectRatio_e::X_MAX_Y_MIN;
	}else if(str == "xMinYMid"){
		return PreserveAspectRatio_e::X_MIN_Y_MID;
	}else if(str == "xMidYMid"){
		return PreserveAspectRatio_e::X_MID_Y_MID;
	}else if(str == "xMaxYMid"){
		return PreserveAspectRatio_e::X_MAX_Y_MID;
	}else if(str == "xMinYMax"){
		return PreserveAspectRatio_e::X_MIN_Y_MAX;
	}else if(str == "xMidYMax"){
		return PreserveAspectRatio_e::X_MID_Y_MAX;
	}else if(str == "xMaxYMax"){
		return PreserveAspectRatio_e::X_MAX_Y_MAX;
	}
	return PreserveAspectRatio_e::NONE;
}
}

namespace{
std::string preserveAspectRatioToString(PreserveAspectRatio_e par){
	switch(par){
		default:
			ASSERT(false)
			return std::string();
		case PreserveAspectRatio_e::NONE:
			return "none";
		case PreserveAspectRatio_e::X_MIN_Y_MIN:
			return "xMinYMin";
		case PreserveAspectRatio_e::X_MID_Y_MIN:
			return "xMidYMin";
		case PreserveAspectRatio_e::X_MAX_Y_MIN:
			return "xMaxYMin";
		case PreserveAspectRatio_e::X_MIN_Y_MID:
			return "xMinYMid";
		case PreserveAspectRatio_e::X_MID_Y_MID:
			return "xMidYMid";
		case PreserveAspectRatio_e::X_MAX_Y_MID:
			return "xMaxYMid";
		case PreserveAspectRatio_e::X_MIN_Y_MAX:
			return "xMinYMax";
		case PreserveAspectRatio_e::X_MID_Y_MAX:
			return "xMidYMax";
		case PreserveAspectRatio_e::X_MAX_Y_MAX:
			return "xMaxYMax";
	}
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

void ViewBoxed::attribsToStream(std::ostream& s)const {
	if (this->isViewBoxSpecified()) {
		s << " viewBox=\"";

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
		s << "\"";
	}

	if (this->preserveAspectRatio.preserve != PreserveAspectRatio_e::NONE || this->preserveAspectRatio.defer || this->preserveAspectRatio.slice) {
		s << " preserveAspectRatio=\"";
		if (this->preserveAspectRatio.defer) {
			s << "defer ";
		}

		s << preserveAspectRatioToString(this->preserveAspectRatio.preserve);

		if (this->preserveAspectRatio.slice) {
			s << " slice";
		}
		s << "\"";
	}
}
