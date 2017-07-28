#include "Structurals.hpp"

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;


void GElement::attribsToStream(std::ostream& s) const{
	this->Element::attribsToStream(s);
	this->Transformable::attribsToStream(s);
	this->Styleable::attribsToStream(s);
}

void SvgElement::attribsToStream(std::ostream& s) const {
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	this->Rectangle::attribsToStream(s);
	this->ViewBoxed::attribsToStream(s);
}

void SymbolElement::attribsToStream(std::ostream& s) const {
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	this->ViewBoxed::attribsToStream(s);
}

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
