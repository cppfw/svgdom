#include "text_element.hpp"

#include "../visitor.hpp"

using namespace svgdom;

void text_element::accept(visitor& v) {
	v.visit(*this);
}

void text_element::accept(const_visitor& v) const{
	v.visit(*this);
}
