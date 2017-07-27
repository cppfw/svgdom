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
	
	/**
	 * @brief Get ID of the locally referenced element.
	 * @return ID of the locally referenced element.
	 * @return Empty string if this Referencing does not refer to any element or the reference is not local IRI.
	 */
	std::string getLocalIri()const;
	
	void attribsToStream(std::ostream& s)const;
};

}
