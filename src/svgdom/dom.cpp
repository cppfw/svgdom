#include "dom.hpp"

#include <pugixml.hpp>

#include <map>

using namespace svgdom;


namespace{

enum class EXmlNamespace{
	UNKNOWN,
	SVG,
	XLINK
};

const char* DSvgNamespace = "http://www.w3.org/2000/svg";
const char* DXlinkNamespace = "http://www.w3.org/1999/xlink";


struct Parser{
	std::map<std::string, EXmlNamespace> namespaces;
	
	EXmlNamespace defaultNamespace = EXmlNamespace::UNKNOWN;
	
	
	std::unique_ptr<svgdom::Element> parseNode(const pugi::xml_node& n);

	std::unique_ptr<SvgElement> parseSvgElement(const pugi::xml_node& n){
		//TODO:
		return nullptr;
	}
};//~class


std::unique_ptr<svgdom::Element> Parser::parseNode(const pugi::xml_node& n){
	//parse default namespace
	{
		pugi::xml_attribute dn = n.attribute("xmlns");
		if(!dn.empty()){
			if(std::string(dn.value()) == DSvgNamespace){
				this->defaultNamespace = EXmlNamespace::SVG;
			}else{
				this->defaultNamespace = EXmlNamespace::UNKNOWN;
			}
		}
	}
	
	//parse other namespaces
	{
		std::string xmlns = "xmlns:";
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto attr = std::string(a.name());
			
			if(attr.substr(0, xmlns.length()) != xmlns){
				continue;
			}
			
			ASSERT(attr.length() >= xmlns.length())
			auto ns = attr.substr(xmlns.length(), attr.length() - xmlns.length());
			
			if(ns == DSvgNamespace){
				this->namespaces[ns] = EXmlNamespace::SVG;
			}else if (ns == DXlinkNamespace){
				this->namespaces[ns] = EXmlNamespace::XLINK;
			}else{
				this->namespaces.erase(ns);
			}
		}
	}
	
	
	if(std::string("svg") == n.value()){
		return parseSvgElement(n);
	}
	
	
	return nullptr;
}

}//~namespace



std::unique_ptr<SvgElement> svgdom::load(const papki::File& f){

	auto fileContents = f.loadWholeFileIntoMemory();
	
	pugi::xml_document doc;
	doc.load_buffer(&*fileContents.begin(), fileContents.size());
	
	Parser parser;
	
	auto ret = parser.parseNode(doc.first_child());
	
	return std::unique_ptr<SvgElement>(dynamic_cast<SvgElement*>(ret.release()));
}
