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
	s = skip_whitespaces(s);

	auto w = read_word(s);
	s = w.view;

	s = skip_whitespaces(s);
	
	if(w.word == "defer"){
		this->defer = true;
		w = read_word(s);
		if(w.word.empty()){
			return;
		}
		s = w.view;
	}else{
		this->defer = false;
	}

	this->preserve = string_to_preserve_aspect_ratio(w.word);

	w = read_word(s);
	if(w.word.empty()){
		return;
	}

	if(w.word == "meet"){
		this->slice = false;
	}else if(w.word == "slice"){
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
