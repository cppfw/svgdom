#pragma once

#include <unordered_map>

#include "../elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class elements_by_tag_name_finder{
public:

	elements_by_tag_name_finder(const svgdom::element& root);

	const std::vector<const element*> find(const std::string& tag_name)const;

	/**
	 * @brief Get elements-by-tag-name cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

private:
	std::unordered_map<std::string, std::vector< const element*>> cache;
};

}
