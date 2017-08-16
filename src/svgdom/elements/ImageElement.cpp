#include "ImageElement.hpp"

#include "../Visitor.hpp"

using namespace svgdom;

void ImageElement::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void ImageElement::accept(ConstVisitor& visitor) const{
	visitor.visit(*this);
}
