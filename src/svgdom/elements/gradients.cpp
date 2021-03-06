#include "gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

const std::string gradient::stop_element::tag = "stop";
const std::string linear_gradient_element::tag = "linearGradient";
const std::string radial_gradient_element::tag = "radialGradient";

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
	switch(this->spread_method_){
		default:
		case gradient::spread_method::default_:
			return "";
		case gradient::spread_method::pad:
			return "pad";
		case gradient::spread_method::reflect:
			return "reflect";
		case gradient::spread_method::repeat:
			return "repeat";
	}
}
