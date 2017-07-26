#include "Container.hpp"

using namespace svgdom;


Element* Container::findById(const std::string& elementId) {
	if(auto e = this->Element::findById(elementId)){
		return e;
	}
	
	for(auto& c : this->children){
		if(auto e = c->findById(elementId)){
			return e;
		}
	}
	return nullptr;
}


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
