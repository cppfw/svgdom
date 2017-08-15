#include "Gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;


void RadialGradientElement::accept(Visitor& visitor){
	visitor.visit(*this);
}

void RadialGradientElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void LinearGradientElement::accept(Visitor& visitor){
	visitor.visit(*this);
}

void LinearGradientElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Gradient::StopElement::accept(Visitor& visitor){
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
