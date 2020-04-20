#include "structurals.hpp"

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

std::array<real, 2> svg_element::get_dimensions(real dpi) const noexcept{
	real w = this->width.toPx(dpi);
	real h = this->height.toPx(dpi);

	if(w <= 0){
		if(this->viewBox[2] > 0){
			w = this->viewBox[2];
		}else{
			w = 300;
		}
	}
	
	if(h <= 0){
		if(this->viewBox[3] > 0){
			h = this->viewBox[3];
		}else{
			h = 150;
		}
	}
	
	return {{w, h}};
}


real svg_element::aspect_ratio(real dpi)const{
	auto wh = this->getDimensions(dpi);
	
	if(wh[0] <= 0 || wh[1] <= 0){
		return 0;
	}
	
	return wh[0] / wh[1];
}

void g_element::accept(visitor& v){
	v.visit(*this);
}

void g_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void svg_element::accept(visitor& v){
	v.visit(*this);
}

void svg_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void symbol_element::accept(visitor& v){
	v.visit(*this);
}

void symbol_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void defs_element::accept(visitor& v){
	v.visit(*this);
}

void defs_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void use_element::accept(visitor& v){
	v.visit(*this);
}

void use_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void mask_element::accept(visitor& v){
	v.visit(*this);
}

void mask_element::accept(const_visitor& v) const {
	v.visit(*this);
}
