#include "Element.hpp"

#include <ostream>
#include <sstream>

#include "Container.hpp"
#include "../Visitor.hpp"
#include "../StreamWriter.hxx"

using namespace svgdom;


void Element::attribsToStream(std::ostream& s) const{
	if(this->id.length() != 0){
		s << " id=\"" << this->id << "\"";
	}
}


std::string Element::toString() const{
	std::stringstream s;
	
	StreamWriter visitor(s);
	this->accept(visitor);
	
	return s.str();
}
