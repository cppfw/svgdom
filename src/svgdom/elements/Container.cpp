#include "Container.hpp"
#include "../Visitor.hpp"

using namespace svgdom;



void Container::relayAccept(Visitor& visitor) const{
	for(auto& e : this->children){
		e->accept(visitor);
	}
}
