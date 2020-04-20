#include "TextElement.hpp"

#include "../visitor.hpp"

using namespace svgdom;

void TextElement::accept(visitor& v) {
	v.visit(*this);
}

void TextElement::accept(const_visitor& v) const{
	v.visit(*this);
}
