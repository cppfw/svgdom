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

	const element_info* find_by_id(const std::string& id)const;
	
	/**
	 * @brief Get cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

private:
	std::map<std::string, element_info> cache;
};

}
