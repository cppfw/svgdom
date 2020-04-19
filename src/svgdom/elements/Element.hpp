#pragma once

#include <ostream>

#include <utki/Unique.hpp>

namespace svgdom{

class visitor;
class const_visitor;

/**
 * @brief Base class for all SVG document elements.
 */
struct Element{
	std::string id;
	
	std::string toString()const;
	
	/**
	 * @brief Accept method for visitor pattern.
	 * @param v - visitor to accept.
	 */
	virtual void accept(visitor& v) = 0;
	
	/**
	 * @brief Accept method for visitor pattern.
	 * @param v - constant visitor to accept.
	 */
	virtual void accept(const_visitor& v) const = 0;

	virtual ~Element()noexcept{}
};

}
