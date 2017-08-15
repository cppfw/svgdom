#pragma once

#include <list>

#include "Element.hpp"

namespace svgdom{

/**
 * @brief An element which can have child elements.
 */
struct Container{
	std::list<std::unique_ptr<Element>> children;
};

}
