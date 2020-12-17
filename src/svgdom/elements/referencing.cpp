#include "referencing.hpp"

#include "../util.hxx"

using namespace svgdom;

std::string referencing::get_local_id_from_iri() const {
	return iri_to_local_id(this->iri);
}
