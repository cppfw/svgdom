#include "Structurals.hpp"

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;


void GElement::attribsToStream(std::ostream& s) const{
	this->Container::attribsToStream(s);
	this->Transformable::attribsToStream(s);
	this->Styleable::attribsToStream(s);
}

void GElement::toStream(std::ostream& s, unsigned indent) const{
	auto ind = indentStr(indent);
	
	s << ind << "<g";
	this->attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</g>";
	}
	s << std::endl;
}

void GElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void SvgElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void SymbolElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void DefsElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void UseElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}
