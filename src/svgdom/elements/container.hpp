#pragma once

#include <list>

#include "Element.hpp"

namespace svgdom{

/**
 * @brief An element which can have child elements.
 */
struct container{
	std::list<std::unique_ptr<Element>> children;
	
	container() = default;
	
	/**
	 * @brief Copy constructor.
	 * This copy constructor does nothing because to make a deep copy of the children
	 * one needs to clone every child by hand.
	 * @param orig - object to copy.
	 */
	container(const container& orig){}
};

// TODO: deprecated, remove.
typedef container Container;

}
