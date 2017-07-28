#include "Container.hpp"

using namespace svgdom;



void Container::relayAccept(Visitor& visitor) const{
	for(auto& e : this->children){
		e->accept(visitor);
	}
}


void Container::childrenToStream(std::ostream& s, unsigned indent) const{
	for(auto& e : this->children){
		e->toStream(s, indent);
	}
}
