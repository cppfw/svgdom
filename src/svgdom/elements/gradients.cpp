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
	switch(this->spread_method){
		default:
		case gradient_spread_method::default_:
			return "";
		case gradient_spread_method::pad:
			return "pad";
		case gradient_spread_method::reflect:
			return "reflect";
		case gradient_spread_method::repeat:
			return "repeat";
	}
}
