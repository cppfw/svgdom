#include "image_element.hpp"

#include "../visitor.hpp"

using namespace svgdom;

void image_element::accept(visitor& v) {
	v.visit(*this);
}

void image_element::accept(const_visitor& v) const{
	v.visit(*this);
}
