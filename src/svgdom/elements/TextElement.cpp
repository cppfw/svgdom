#include "TextElement.hpp"

#include "../Visitor.hpp"

using namespace svgdom;

void TextElement::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void TextElement::accept(ConstVisitor& visitor) const{
	visitor.visit(*this);
}