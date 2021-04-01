#pragma once

#include <unordered_map>

#include "../elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class style_stack_cache{
public:

	style_stack_cache(const svgdom::element& root);

	const style_stack* find(const std::string& id)const;

	/**
	 * @brief Get style-stack-by-id cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

private:
	std::unordered_map<std::string, style_stack> cache;
};

}
