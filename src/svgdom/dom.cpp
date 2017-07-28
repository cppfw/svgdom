#include <pugixml.hpp>

#include "dom.hpp"
#include "config.hpp"
#include "Exc.hpp"
#include "util.hxx"

#include "Parser.hxx"

using namespace svgdom;



namespace{
std::unique_ptr<SvgElement> load(const pugi::xml_document& doc){
	Parser parser;
	
	//return first node which is successfully parsed
	for(auto n = doc.first_child(); !n.empty(); n = n.next_sibling()){
		auto element = parser.parseNode(doc.first_child());

		if(auto svgElement = dynamic_cast<SvgElement*>(element.get())){
			auto ret = std::unique_ptr<SvgElement>(svgElement);
			element.release();
			return ret;
		}
	}
	return nullptr;
}
}


std::unique_ptr<SvgElement> svgdom::load(const papki::File& f){
	pugi::xml_document doc;
	{
		auto fileContents = f.loadWholeFileIntoMemory();
		if(doc.load_buffer(&*fileContents.begin(), fileContents.size()).status != pugi::xml_parse_status::status_ok){
			TRACE(<< "svgdom::load(): loading XML document failed!" << std::endl)
			return nullptr;
		}
	}
	
	return ::load(doc);
}

std::unique_ptr<SvgElement> svgdom::load(std::istream& s){
	pugi::xml_document doc;
	
	doc.load(s);
	
	return ::load(doc);
}

std::unique_ptr<SvgElement> svgdom::load(std::string& s){
	pugi::xml_document doc;

	doc.load_string(s.c_str());

	return ::load(doc);
}
