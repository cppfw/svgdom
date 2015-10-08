#include "dom.hpp"

#include <pugixml.hpp>


using namespace svgdom;


namespace{
	
std::unique_ptr<svgdom::Element> parseNode(const pugi::xml_node& n){
	return nullptr;
}
	
}



std::unique_ptr<SvgElement> svgdom::load(const papki::File& f){

	auto fileContents = f.loadWholeFileIntoMemory();
	
	pugi::xml_document doc;
	doc.load_buffer(&*fileContents.begin(), fileContents.size());
	
	auto ret = parseNode(doc.first_child());
	
	return std::unique_ptr<SvgElement>(dynamic_cast<SvgElement*>(ret.release()));
}
