#include "Styleable.hpp"

#include <utki/debug.hpp>

using namespace svgdom;


StyleProperty_e Styleable::stringToProperty(std::string str){
	if(str == "fill"){
		return StyleProperty_e::FILL;
	}else if(str == "fill-opacity"){
		return StyleProperty_e::FILL_OPACITY;
	}else if(str == "stroke"){
		return StyleProperty_e::STROKE;
	}else if(str == "stroke-width"){
		return StyleProperty_e::STROKE_WIDTH;
	}else if(str == "stroke-linecap"){
		return StyleProperty_e::STROKE_LINECAP;
	}else if(str == "stroke-linejoin"){
		return StyleProperty_e::STROKE_LINEJOIN;
	}else if(str == "stroke-opacity"){
		return StyleProperty_e::STROKE_OPACITY;
	}else if(str == "opacity"){
		return StyleProperty_e::OPACITY;
	}else if(str == "stop-opacity"){
		return StyleProperty_e::STOP_OPACITY;
	}else if(str == "stop-color"){
		return StyleProperty_e::STOP_COLOR;
	}else if(str == "fill-rule"){
		return StyleProperty_e::FILL_RULE;
	}
	
	return StyleProperty_e::UNKNOWN;
}

std::string Styleable::propertyToString(StyleProperty_e p){
	switch(p){
		default:
			ASSERT(false)
			return "";
		case StyleProperty_e::FILL:
			return "fill";
		case StyleProperty_e::FILL_OPACITY:
			return "fill-opacity";
		case StyleProperty_e::STROKE:
			return "stroke";
		case StyleProperty_e::STROKE_WIDTH:
			return "stroke-width";
		case StyleProperty_e::STROKE_LINECAP:
			return "stroke-linecap";
		case StyleProperty_e::STROKE_LINEJOIN:
			return "stroke-linejoin";
		case StyleProperty_e::STROKE_OPACITY:
			return "stroke-opacity";
		case StyleProperty_e::OPACITY:
			return "opacity";
		case StyleProperty_e::STOP_OPACITY:
			return "stop-opacity";
		case StyleProperty_e::STOP_COLOR:
			return "stop-color";
		case StyleProperty_e::FILL_RULE:
			return "fill-rule";
	}
}
