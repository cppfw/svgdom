#include "Structurals.hpp"

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

std::array<real, 2> SvgElement::getDimensions(real dpi) const noexcept{
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


real SvgElement::aspectRatio(real dpi)const{
	auto wh = this->getDimensions(dpi);
	
	if(wh[0] <= 0 || wh[1] <= 0){
		return 0;
	}
	
	return wh[0] / wh[1];
}

void GElement::accept(visitor& v){
	v.visit(*this);
}

void GElement::accept(ConstVisitor& v) const{
	v.visit(*this);
}

void SvgElement::accept(visitor& v){
	v.visit(*this);
}

void SvgElement::accept(ConstVisitor& v) const{
	v.visit(*this);
}

void SymbolElement::accept(visitor& v){
	v.visit(*this);
}

void SymbolElement::accept(ConstVisitor& v) const {
	v.visit(*this);
}

void DefsElement::accept(visitor& v){
	v.visit(*this);
}

void DefsElement::accept(ConstVisitor& v) const{
	v.visit(*this);
}

void UseElement::accept(visitor& v){
	v.visit(*this);
}

void UseElement::accept(ConstVisitor& v) const {
	v.visit(*this);
}

void MaskElement::accept(visitor& v){
	v.visit(*this);
}

void MaskElement::accept(ConstVisitor& v) const {
	v.visit(*this);
}
