#pragma once

#include <ostream>

#include <utki/Unique.hpp>

#include "Styleable.hpp"

namespace svgdom{

class Visitor;

/**
 * @brief Base class for all SVG document elements.
 */
struct Element :
		public utki::Unique,
		public Styleable //All SVG elements are Styleable according to SVG spec
{
	std::string id;
	
	//TODO: move all attribsToStream to cpp
	void attribsToStream(std::ostream& s)const;
	
	std::string toString()const;
	
	/**
	 * @brief Accept method for Visitor pattern.
	 * @param visitor - visitor to accept.
	 */
	virtual void accept(Visitor& visitor)const = 0;
};

}
