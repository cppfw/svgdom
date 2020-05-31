#pragma once

#include <map>

#include "elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class finder{
public:
	
	finder(const svgdom::element& root);
	
	struct element_info{
		const svgdom::element& e;
		const style_stack ss;
		
		element_info(const svgdom::element& e, style_stack ss) :
				e(e),
				ss(ss)
		{}
	};

	// TODO: DEPRECATED, remove.
	typedef element_info ElementInfo;
	
	const element_info* find_by_id(const std::string& id)const;
	
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
	std::map<std::string, element_info> cache;
};

// TODO: DEPRECATED, remove.
typedef finder Finder;

}
