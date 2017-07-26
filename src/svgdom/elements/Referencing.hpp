#pragma once

//TODO: remove
#include "Element.hpp"

namespace svgdom{

/**
 * @brief an element which can reference another element.
 */
struct Referencing{
	/**
	 * @brief Referenced element.
	 * If the reference is an IRI to an object outside of the SVG document then this variable is nullptr.
	 */
	//TODO: remove
	Element* ref = nullptr;

	/**
	 * @brief IRI reference.
	 * This variable holds the IRI string.
	 */
	std::string iri;
	
	void attribsToStream(std::ostream& s)const;
};

}
