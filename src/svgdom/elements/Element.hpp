#pragma once

#include <ostream>

#include <utki/Unique.hpp>

namespace svgdom{

class Visitor;

/**
 * @brief Base class for all SVG document elements.
 */
struct Element : public utki::Unique
{
	std::string id;
	
	std::string toString()const;
	
	/**
	 * @brief Accept method for Visitor pattern.
	 * @param visitor - visitor to accept.
	 */
	virtual void accept(Visitor& visitor)const = 0;
};

}
