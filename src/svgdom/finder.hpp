#pragma once

#include <map>

#include "elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class finder{
public:
	
	finder(const svgdom::Element& root);
	
	struct element_info{
		const svgdom::Element& e;
		const StyleStack ss;
		
		element_info(const svgdom::Element& e, StyleStack ss) : e(e), ss(ss){}
	};

	// TODO: DEPRECATED, remove.
	typedef element_info ElementInfo;
	
	const ElementInfo* find_by_id(const std::string& id)const;
	
	// TODO: DEPRECATED, remove.
	const ElementInfo* findById(const std::string& id)const{
		return this->find_by_id(id);
	}

	/**
	 * @brief Get cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

	// TODO: DEPRECATED, remove.
	size_t cacheSize()const noexcept{
		return this->size();
	}
	
private:
	std::map<std::string, ElementInfo> cache;
};

// TODO: DEPRECATED, remove.
typedef finder Finder;

}
