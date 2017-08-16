#include "AspectRatioed.hpp"

#include <sstream>

using namespace svgdom;

namespace{

AspectRatioed::PreserveAspectRatio_e stringToPreserveAspectRatio(const std::string& str){
	if(str == "none"){
		return AspectRatioed::PreserveAspectRatio_e::NONE;
	}else if(str == "xMinYMin"){
		return AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MIN;
	}else if(str == "xMidYMin"){
		return AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MIN;
	}else if(str == "xMaxYMin"){
		return AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MIN;
	}else if(str == "xMinYMid"){
		return AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MID;
	}else if(str == "xMidYMid"){
		return AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MID;
	}else if(str == "xMaxYMid"){
		return AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MID;
	}else if(str == "xMinYMax"){
		return AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MAX;
	}else if(str == "xMidYMax"){
		return AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MAX;
	}else if(str == "xMaxYMax"){
		return AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MAX;
	}
	return AspectRatioed::PreserveAspectRatio_e::NONE;
}
}


void AspectRatioed::parseAndFillPreserveAspectRatio(const std::string& str) {
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

std::string AspectRatioed::preserveAspectRatioToString() const {
	std::stringstream s;
	if (this->preserveAspectRatio.defer) {
		s << "defer ";
	}

	switch(this->preserveAspectRatio.preserve){
		default:
			break;
		case AspectRatioed::PreserveAspectRatio_e::NONE:
			s << "none";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MIN:
			s << "xMinYMin";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MIN:
			s << "xMidYMin";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MIN:
			s << "xMaxYMin";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MID:
			s << "xMinYMid";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MID:
			s << "xMidYMid";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MID:
			s << "xMaxYMid";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MIN_Y_MAX:
			s << "xMinYMax";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MID_Y_MAX:
			s << "xMidYMax";
			break;
		case AspectRatioed::PreserveAspectRatio_e::X_MAX_Y_MAX:
			s << "xMaxYMax";
			break;
	}

	if (this->preserveAspectRatio.slice) {
		s << " slice";
	}
	return s.str();
}
