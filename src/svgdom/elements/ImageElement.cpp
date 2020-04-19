#include "ImageElement.hpp"

#include "../visitor.hpp"

using namespace svgdom;

void ImageElement::accept(visitor& v) {
	v.visit(*this);
}

void ImageElement::accept(ConstVisitor& v) const{
	v.visit(*this);
}
