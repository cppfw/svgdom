#include "Shapes.hpp"

#include "../Visitor.hpp"

using namespace svgdom;

void PathElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void RectElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void CircleElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void EllipseElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void LineElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void PolygonElement::accept(Visitor& visistor) const {
	visistor.visit(*this);
}

void PolylineElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}
