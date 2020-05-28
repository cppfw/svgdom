#include "style.hpp"

#include "../visitor.hpp"

using namespace svgdom;

const std::string style_element::tag = "style";

void style_element::accept(visitor& v){
	v.visit(*this);
}

void style_element::accept(const_visitor& v)const{
	v.visit(*this);
}
