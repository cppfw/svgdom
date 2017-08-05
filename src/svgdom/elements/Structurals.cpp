#include "Structurals.hpp"

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;



real SvgElement::aspectRatio(real dpi)const{
	real w = this->width.toPx(dpi);
	real h = this->height.toPx(dpi);
	
	if(w <= 0 || h <= 0){
		if(this->viewBox[2] <= 0 || this->viewBox[3] <= 0){
			return 0;
		}else{
			return this->viewBox[2] / this->viewBox[3];
		}
	}
	
	return w / h;
}

void GElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void SvgElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void SymbolElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void DefsElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void UseElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}
