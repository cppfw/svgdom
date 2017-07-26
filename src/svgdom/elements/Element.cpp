#include "Element.hpp"

#include <sstream>

#include "Container.hpp"

using namespace svgdom;


void Element::attribsToStream(std::ostream& s) const{
	if(this->id.length() != 0){
		s << " id=\"" << this->id << "\"";
	}
}

std::string Element::toString() const{
	std::stringstream s;
	this->toStream(s, 0);
	return s.str();
}

const StylePropertyValue* Element::getStyleProperty(StyleProperty_e property, bool explicitInherit) const{
	if(auto styleable = dynamic_cast<const Styleable*>(this)){
		if(auto p = styleable->findStyleProperty(property)){
			if(p->type == StylePropertyValue::Type_e::INHERIT){
				explicitInherit = true;
			}else{
				return p;
			}
		}
	}
	
	if(!explicitInherit && !Styleable::isStylePropertyInherited(property)){
		return nullptr;
	}
	
	if(!this->parent){
		return nullptr;
	}
	
	return this->parent->getStyleProperty(property, explicitInherit);
}

Element* Element::findById(const std::string& elementId) {
	if(this->id == elementId){
		return this;
	}
	return nullptr;
}
