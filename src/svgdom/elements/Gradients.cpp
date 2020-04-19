#include "Gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;


void RadialGradientElement::accept(visitor& visitor){
	visitor.visit(*this);
}

void RadialGradientElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void LinearGradientElement::accept(visitor& visitor){
	visitor.visit(*this);
}

void LinearGradientElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Gradient::StopElement::accept(visitor& visitor){
	visitor.visit(*this);
}

void Gradient::StopElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

std::string Gradient::spreadMethodToString() const {
	switch(this->spreadMethod){
		default:
		case Gradient::SpreadMethod_e::DEFAULT:
			return "";
		case Gradient::SpreadMethod_e::PAD:
			return "pad";
		case Gradient::SpreadMethod_e::REFLECT:
			return "reflect";
		case Gradient::SpreadMethod_e::REPEAT:
			return "repeat";
	}
}
