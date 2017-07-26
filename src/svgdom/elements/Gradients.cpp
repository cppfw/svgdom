#include "Gradients.hpp"

#include "../Visitor.hpp"

using namespace svgdom;


void RadialGradientElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void LinearGradientElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void Gradient::StopElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}
