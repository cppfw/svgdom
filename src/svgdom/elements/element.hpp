#pragma once

#include <ostream>

namespace svgdom{

class visitor;
class const_visitor;

/**
 * @brief Base class for all SVG document elements.
 */
struct element{
	std::string id;
	
	std::string to_string()const;

	/**
	 * @brief Accept method for visitor pattern.
	 * @param v - visitor to accept.
	 */
	virtual void accept(visitor& v) = 0;
	
	/**
	 * @brief Accept method for visitor pattern.
	 * @param v - constant visitor to accept.
	 */
	virtual void accept(const_visitor& v)const = 0;

	/**
	 * @brief Get tag name of the element.
	 * @return Tag name of the element.
	 */
	virtual const std::string& get_tag()const = 0;

	virtual ~element()noexcept{}
};

}
