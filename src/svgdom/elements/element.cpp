#include "element.hpp"

#include <ostream>
#include <sstream>

#include "container.hpp"
#include "../visitor.hpp"
#include "../stream_writer.hpp"

using namespace svgdom;

std::string element::to_string()const{
	std::stringstream s;
	
	stream_writer visitor(s);
	this->accept(visitor);
	
	return s.str();
}
