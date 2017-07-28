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

void LinearGradientElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	
	s << ind << "<linearGradient";
	this->attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</linearGradient>";
	}
	s << std::endl;
}

void LinearGradientElement::attribsToStream(std::ostream& s) const{
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
}

void RadialGradientElement::attribsToStream(std::ostream& s) const {
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
}


void RadialGradientElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	
	s << ind << "<radialGradient";
	this->attribsToStream(s);
	
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
	this->Element::attribsToStream(s);
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

void Gradient::StopElement::attribsToStream(std::ostream& s) const {
	s << " offset=\"" << this->offset << "\"";
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
}

void Gradient::StopElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);
	s << ind << "<stop";
	this->attribsToStream(s);
	s << "/>" << std::endl;
}
