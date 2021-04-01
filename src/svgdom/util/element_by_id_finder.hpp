#pragma once

#include <unordered_map>

#include "../elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class element_by_id_finder{
public:

	element_by_id_finder(const svgdom::element& root);

	const element* find(const std::string& id)const;

	/**
	 * @brief Get element-by-id cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

private:
	std::unordered_map<std::string, const element*> cache;
};

}
