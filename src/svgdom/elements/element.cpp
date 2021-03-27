#include "element.hpp"

#include <ostream>
#include <sstream>

#include "container.hpp"
#include "../visitor.hpp"
#include "../stream_writer.hpp"

using namespace svgdom;

std::string element::classes_to_string()const{
    std::stringstream ss;

    for(auto i = this->classes.begin(); i != this->classes.end(); ++i){
        if(i != this->classes.begin()){
            ss << " ";
        }
        ss << *i;
    }

    return ss.str();
}

std::string element::to_string()const{
	std::stringstream s;
	
	stream_writer visitor(s);
	this->accept(visitor);
	
	return s.str();
}
