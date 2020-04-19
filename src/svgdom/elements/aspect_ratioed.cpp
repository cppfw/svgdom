#include "aspect_ratioed.hpp"

#include <sstream>

using namespace svgdom;

namespace{
aspect_ratioed::aspect_ratio_preservation stringToPreserveAspectRatio(const std::string& str){
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

void decltype(aspect_ratioed::preserve_aspect_ratio)::parse(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	std::string tmp;
	
	s >> tmp;
	
	if(s.fail()){
		return;
	}
	
	if(tmp == "defer"){
		this->defer = true;
		s >> tmp;
		if(s.fail()){
			return;
		}
	}else{
		this->defer = false;
	}
	
	this->preserve = stringToPreserveAspectRatio(tmp);
	
	s >> tmp;
	if(s.fail()){
		return;
	}
	
	if(tmp == "meet"){
		this->slice = false;
	}else if(tmp == "slice"){
		this->slice = true;
	}
}

std::string decltype(aspect_ratioed::preserve_aspect_ratio)::to_string()const{
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
