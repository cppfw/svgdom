#include "Element.hpp"

#include <ostream>
#include <sstream>

#include "container.hpp"
#include "../visitor.hpp"
#include "../stream_writer.hpp"

using namespace svgdom;

std::string Element::toString() const{
	std::stringstream s;
	
	StreamWriter visitor(s);
	this->accept(visitor);
	
	return s.str();
}
