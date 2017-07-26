#include "Gradients.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;



namespace{
std::string gradientSpreadMethodToString(Gradient::SpreadMethod_e sm) {
	switch(sm){
		default:
			ASSERT_INFO(false, "sm = " << unsigned(sm))
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
}

void RadialGradientElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void LinearGradientElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void Gradient::StopElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

Gradient::SpreadMethod_e Gradient::getSpreadMethod() const noexcept{
	if(this->spreadMethod != SpreadMethod_e::DEFAULT){
		return this->spreadMethod;
	}
	
	ASSERT(this->spreadMethod == SpreadMethod_e::DEFAULT)
	
	if(this->ref){
		if(auto g = dynamic_cast<Gradient*>(this->ref)){
			return g->getSpreadMethod();
		}
	}
	
	return SpreadMethod_e::PAD;
}

const decltype(Container::children)& Gradient::getStops() const noexcept{
	if(this->children.size() != 0){
		return this->children;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<Gradient*>(this->ref)){
			return g->getStops();
		}
	}
	return this->children;
}

Length LinearGradientElement::getX1() const noexcept{
	if(this->x1.unit != Length::Unit_e::UNKNOWN){
		return this->x1;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<LinearGradientElement*>(this->ref)){
			return g->getX1();
		}
	}
	return Length::make(0, Length::Unit_e::PERCENT);
}

Length LinearGradientElement::getX2() const noexcept{
	if(this->x2.unit != Length::Unit_e::UNKNOWN){
		return this->x2;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<LinearGradientElement*>(this->ref)){
			return g->getX2();
		}
	}
	return Length::make(100, Length::Unit_e::PERCENT);
}

Length LinearGradientElement::getY1() const noexcept{
	if(this->y1.unit != Length::Unit_e::UNKNOWN){
		return this->y1;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<LinearGradientElement*>(this->ref)){
			return g->getY1();
		}
	}
	return Length::make(0, Length::Unit_e::PERCENT);
}

Length LinearGradientElement::getY2() const noexcept{
	if(this->y2.unit != Length::Unit_e::UNKNOWN){
		return this->y2;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<LinearGradientElement*>(this->ref)){
			return g->getY2();
		}
	}
	return Length::make(0, Length::Unit_e::PERCENT);
}

Length RadialGradientElement::getCx() const noexcept{
	if(this->cx.unit != Length::Unit_e::UNKNOWN){
		return this->cx;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<RadialGradientElement*>(this->ref)){
			return g->getCx();
		}
	}
	return Length::make(50, Length::Unit_e::PERCENT);
}

Length RadialGradientElement::getCy() const noexcept{
	if(this->cy.unit != Length::Unit_e::UNKNOWN){
		return this->cy;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<RadialGradientElement*>(this->ref)){
			return g->getCy();
		}
	}
	return Length::make(50, Length::Unit_e::PERCENT);
}

Length RadialGradientElement::getR() const noexcept{
	if(this->r.unit != Length::Unit_e::UNKNOWN){
		return this->r;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<RadialGradientElement*>(this->ref)){
			return g->getR();
		}
	}
	return Length::make(50, Length::Unit_e::PERCENT);
}

Length RadialGradientElement::getFx() const noexcept{
	if(this->fx.unit != Length::Unit_e::UNKNOWN){
		return this->fx;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<RadialGradientElement*>(this->ref)){
			return g->getFx();
		}
	}
	return Length::make(0, Length::Unit_e::UNKNOWN);
}

Length RadialGradientElement::getFy() const noexcept{
	if(this->fy.unit != Length::Unit_e::UNKNOWN){
		return this->fy;
	}
	
	if(this->ref){
		if(auto g = dynamic_cast<RadialGradientElement*>(this->ref)){
			return g->getFy();
		}
	}
	return Length::make(0, Length::Unit_e::UNKNOWN);
}

void LinearGradientElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	
	s << ind << "<linearGradient";
	this->Gradient::attribsToStream(s);
	
	if(this->x1.unit != Length::Unit_e::PERCENT || this->x1.value != 0){
		s << " x1=\"" << this->x1 << "\"";
	}
	
	if(this->y1.unit != Length::Unit_e::PERCENT || this->y1.value != 0){
		s << " y1=\"" << this->y1 << "\"";
	}
	
	if(this->x2.unit != Length::Unit_e::PERCENT || this->x2.value != 100){
		s << " x2=\"" << this->x2 << "\"";
	}
	
	if(this->y2.unit != Length::Unit_e::PERCENT || this->y2.value != 0){
		s << " y2=\"" << this->y2 << "\"";
	}
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</linearGradient>";
	}
	s << std::endl;
}

void RadialGradientElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	
	s << ind << "<radialGradient";
	this->Gradient::attribsToStream(s);
	
	if(this->cx.unit != Length::Unit_e::PERCENT || this->cx.value != 50){
		s << " cx=\"" << this->cx << "\"";
	}
	
	if(this->cy.unit != Length::Unit_e::PERCENT || this->cy.value != 50){
		s << " cy=\"" << this->cy << "\"";
	}
	
	if(this->r.unit != Length::Unit_e::PERCENT || this->r.value != 50){
		s << " r=\"" << this->r << "\"";
	}
	
	if(this->fx.unit != Length::Unit_e::UNKNOWN){
		s << " fx=\"" << this->fx << "\"";
	}
	
	if(this->fy.unit != Length::Unit_e::UNKNOWN){
		s << " fy=\"" << this->fy << "\"";
	}
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</radialGradient>";
	}
	s << std::endl;
}

void Gradient::attribsToStream(std::ostream& s)const{
	this->Container::attribsToStream(s);
	this->Referencing::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	
	if(this->spreadMethod != SpreadMethod_e::DEFAULT){
		s << " spreadMethod=\"" << gradientSpreadMethodToString(this->spreadMethod) << "\"";
	}
	
	if(this->units != Units_e::OBJECT_BOUNDING_BOX){
		ASSERT(this->units == Units_e::USER_SPACE_ON_USE)
		s << " gradientUnits=\"userSpaceOnUse\"";
	}
	
	if(this->transformations.size() != 0){
		s << " gradientTransform=\"";
		this->transformationsToStream(s);
		s << "\"";
	}
}


void Gradient::StopElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	s << ind << "<stop";
	s << " offset=\"" << this->offset << "\"";
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	s << "/>" << std::endl;
}
