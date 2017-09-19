#include "Referencing.hpp"

#include "../util.hxx"

using namespace svgdom;



std::string Referencing::getLocalIdFromIri() const {
	return iriToLocalId(this->iri);
}

