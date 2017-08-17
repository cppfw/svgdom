#pragma once

#include <list>

#include "Element.hpp"

namespace svgdom{

/**
 * @brief An element which can have child elements.
 */
struct Container{
	std::list<std::unique_ptr<Element>> children;
	
	Container(){}
	
	/**
	 * @brief Copy constructor.
	 * This copy constructor does nothing because to make a deep copy of the children
	 * one needs to clone every child by hand.
	 * @param orig - object to copy.
	 */
	Container(const Container& orig){}
};

}
