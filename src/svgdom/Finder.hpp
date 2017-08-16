#pragma once

#include <map>

#include "elements/Element.hpp"

#include "StyleStack.hpp"

namespace svgdom{

class Finder : public utki::Unique{
public:
	
	Finder(const svgdom::Element& root);
	
	struct ElementInfo{
		const svgdom::Element& e;
		const StyleStack ss;
		
		ElementInfo(const svgdom::Element& e, StyleStack ss) : e(e), ss(ss){}
	};
	
	const ElementInfo* findById(const std::string& id)const;
	
	size_t cacheSize()const noexcept{
		return this->cache.size();
	}
	
private:
	std::map<std::string, ElementInfo> cache;
};
	
}
