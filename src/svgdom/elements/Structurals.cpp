#include "Structurals.hpp"

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;


void GElement::attribsToStream(std::ostream& s) const{
	this->Element::attribsToStream(s);
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

void DefsElement::toStream(std::ostream& s, unsigned indent) const{
	auto ind = indentStr(indent);
	
//	TRACE(<< "DefsElement::toStream():" << std::endl)

	s << ind << "<defs";
	this->Element::attribsToStream(s);
	this->Transformable::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</defs>";
	}
	s << std::endl;
}

void UseElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);

//	TRACE(<< "UseElement::toStream():" << std::endl)

	s << ind << "<use";
	this->Element::attribsToStream(s);
	this->Transformable::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	this->Rectangle::attribsToStream(s);
	this->Referencing::attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void SvgElement::attribsToStream(std::ostream& s) const {
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	this->Rectangle::attribsToStream(s);
	this->ViewBoxed::attribsToStream(s);
}

void SymbolElement::attribsToStream(std::ostream& s) const {
	this->Element::attribsToStream(s);
	this->Styleable::attribsToStream(s);
	this->ViewBoxed::attribsToStream(s);
}

void SvgElement::toStream(std::ostream& s, unsigned indent) const{
	auto ind = indentStr(indent);
	
	s << ind << "<svg";
	if(indent == 0){//if outermost "svg" element
		s << " xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"";
	}
	this->attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</svg>";
	}
	s << std::endl;
}

real SvgElement::aspectRatio(real dpi)const{
	real w = this->width.toPx(dpi);
	real h = this->height.toPx(dpi);
	
	if(w <= 0 || h <= 0){
		if(this->viewBox[2] <= 0 || this->viewBox[3] <= 0){
			return 0;
		}else{
			return this->viewBox[2] / this->viewBox[3];
		}
	}
	
	return w / h;
}

void SymbolElement::toStream(std::ostream& s, unsigned indent) const {
	auto ind = indentStr(indent);

	s << ind << "<symbol";
	
	this->attribsToStream(s);

	if (this->children.size() == 0) {
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</symbol>";
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
