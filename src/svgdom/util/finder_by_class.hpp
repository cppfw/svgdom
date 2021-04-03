#pragma once

#include <unordered_map>

#include <utki/span.hpp>

#include "../elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class finder_by_class{
public:

	finder_by_class(const svgdom::element& root);

	utki::span<const element* const> find(const std::string& cls)const noexcept;

	/**
	 * @brief Get elements-by-class-name cache size.
	 * @return number of cached elements.
	 */
	size_t size()const noexcept{
		return this->cache.size();
	}

private:
	std::unordered_map<std::string, std::vector< const element*>> cache;
};

}
