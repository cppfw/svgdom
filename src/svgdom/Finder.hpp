#pragma once

#include <map>

#include "elements/Element.hpp"

#include "StyleStack.hpp"

namespace svgdom{

class Finder{
	
	const svgdom::Element& root;
	
public:
	
	Finder(const svgdom::Element& root);
	
	struct ElementInfo{
		const svgdom::Element* e = nullptr;
		StyleStack ss;
		
		explicit operator bool()const{
			return this->e != nullptr;
		}
	};
	
	const ElementInfo& findById(const std::string& id);
	
	void clearCache();
	
private:
	std::map<std::string, ElementInfo> cache;
};
	
}
