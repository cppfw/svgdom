#pragma once

#include <string>

namespace svgdom{

/**
 * @brief an element which can reference another element.
 */
struct referencing{
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
	std::string get_local_id_from_iri()const;
};

}
