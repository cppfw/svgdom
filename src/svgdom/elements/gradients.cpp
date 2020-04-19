#include "gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

void radial_gradient_element::accept(visitor& v){
	v.visit(*this);
}

void radial_gradient_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void linear_gradient_element::accept(visitor& v){
	v.visit(*this);
}

void linear_gradient_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void gradient::stop_element::accept(visitor& v){
	v.visit(*this);
}

void gradient::stop_element::accept(const_visitor& v) const {
	v.visit(*this);
}

std::string gradient::spread_method_to_string() const {
	switch(this->spreadMethod){
		default:
		case gradient::spread_method_kind::default_kind:
			return "";
		case gradient::spread_method_kind::pad:
			return "pad";
		case gradient::spread_method_kind::reflect:
			return "reflect";
		case gradient::spread_method_kind::repeat:
			return "repeat";
	}
}
