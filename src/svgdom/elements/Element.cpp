#include "Element.hpp"

#include <ostream>
#include <sstream>

#include "Container.hpp"
#include "../Visitor.hpp"
#include "../StreamWriter.hpp"

using namespace svgdom;



std::string Element::toString() const{
	std::stringstream s;
	
	StreamWriter visitor(s);
	this->accept(visitor);
	
	return s.str();
}
