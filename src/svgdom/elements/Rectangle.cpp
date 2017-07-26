#include "Rectangle.hpp"

#include <ostream>

using namespace svgdom;


void Rectangle::attribsToStream(std::ostream& s)const{
	if(this->x.value != 0){
		s << " x=\"" << this->x << "\"";
	}
	
	if(this->y.value != 0){
		s << " y=\"" << this->y << "\"";
	}
	
	if(this->width.value != 100 || this->width.unit != Length::Unit_e::PERCENT){ //if width is not 100% (default value)
		s << " width=\"" << this->width << "\"";
	}
	
	if(this->height.value != 100 || this->height.unit != Length::Unit_e::PERCENT){ //if height is not 100% (default value)
		s << " height=\"" << this->height << "\"";
	}
}
