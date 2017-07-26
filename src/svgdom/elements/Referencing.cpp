#include "Referencing.hpp"

using namespace svgdom;


void Referencing::attribsToStream(std::ostream& s) const {
	if(this->iri.length() == 0 && !this->ref){
		return;
	}
	s << " xlink:href=\"";
	
	if(this->ref){
		s << '#' << this->ref->id;
	}else{
		s << this->iri;
	}
	
	s << "\"";
}
