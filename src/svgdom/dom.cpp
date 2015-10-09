#include "dom.hpp"
#include "config.hpp"

#include <pugixml.hpp>

#include <map>
#include <sstream>

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
	typedef std::map<std::string, EXmlNamespace> T_NamespaceMap;
	std::vector<T_NamespaceMap> namespaces;
	
	std::vector<EXmlNamespace> defaultNamespace;
	
	
	EXmlNamespace findNamespace(const std::string& ns){
		for(auto i = this->namespaces.rbegin(); i != this->namespaces.rend(); ++i){
			auto iter = i->find(ns);
			if(iter == i->end()){
				continue;
			}
			ASSERT(ns == iter->first)
			return iter->second;
		}
		return EXmlNamespace::UNKNOWN;
	}
	
	struct NamespaceNamePair{
		EXmlNamespace ns;
		std::string name;
	};
	
	NamespaceNamePair getNamespace(const std::string& fullName){
		NamespaceNamePair ret;
		
		auto colonIndex = fullName.find_first_of(':');
		if(colonIndex == std::string::npos){
			ret.ns = this->defaultNamespace.back();
			ret.name = fullName;
			return ret;
		}
		
		ASSERT(fullName.length() >= colonIndex + 1)
		
		ret.ns = this->findNamespace(fullName.substr(0, colonIndex));
		ret.name = fullName.substr(colonIndex + 1, fullName.length() - 1 - colonIndex);
		
		return ret;
	}
	
	std::unique_ptr<svgdom::Element> parseNode(const pugi::xml_node& n);

	void fillElement(Element& e, const pugi::xml_node& n){
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case EXmlNamespace::SVG:
					if(nsn.name == "id"){
						e.id = a.value();
					}
					break;
				default:
					break;
			}
		}
	}

	void fillRectangle(Rectangle& r, const pugi::xml_node& n){
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case EXmlNamespace::SVG:
					if(nsn.name == "x"){
						r.x = Length::parse(a.value());
					}else if(nsn.name == "y"){
						r.y = Length::parse(a.value());
					}else if(nsn.name == "width"){
						r.width = Length::parse(a.value());
					}else if(nsn.name == "height"){
						r.height = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
	}
	
	void fillContainer(Container& c, const pugi::xml_node& n){
		ASSERT(c.children.size() == 0)
		for(auto i = n.first_child(); !i.empty(); i = i.next_sibling()){
			if(auto res = this->parseNode(i)){
				c.children.push_back(std::move(res));
			}
		}
	}
	
	std::unique_ptr<SvgElement> parseSvgElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == EXmlNamespace::SVG)
		ASSERT(getNamespace(n.name()).name == "svg")
		
		auto ret = utki::makeUnique<SvgElement>();
		
		this->fillElement(*ret, n);
		this->fillRectangle(*ret, n);
		this->fillContainer(*ret, n);

		return ret;
	}
	
	std::unique_ptr<GElement> parseGElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == EXmlNamespace::SVG)
		ASSERT(getNamespace(n.name()).name == "g")
		
		auto ret = utki::makeUnique<GElement>();
		
		this->fillElement(*ret, n);
		this->fillContainer(*ret, n);
		
		return ret;
	}
};//~class


std::unique_ptr<svgdom::Element> Parser::parseNode(const pugi::xml_node& n){
	//parse default namespace
	{
		pugi::xml_attribute dn = n.attribute("xmlns");
		if(!dn.empty()){
			if(std::string(dn.value()) == DSvgNamespace){
				this->defaultNamespace.push_back(EXmlNamespace::SVG);
			}else{
				this->defaultNamespace.push_back(EXmlNamespace::UNKNOWN);
			}
		}else{
			this->defaultNamespace.push_back(this->defaultNamespace.back());
		}
	}
	
	//parse other namespaces
	{
		std::string xmlns = "xmlns:";
		
		this->namespaces.push_back(T_NamespaceMap());
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto attr = std::string(a.name());
			
			if(attr.substr(0, xmlns.length()) != xmlns){
				continue;
			}
			
			ASSERT(attr.length() >= xmlns.length())
			auto ns = attr.substr(xmlns.length(), attr.length() - xmlns.length());
			
			if(ns == DSvgNamespace){
				this->namespaces.back()[ns] = EXmlNamespace::SVG;
			}else if (ns == DXlinkNamespace){
				this->namespaces.back()[ns] = EXmlNamespace::XLINK;
			}else{
				this->namespaces.back().erase(ns);
			}
		}
	}
	
	auto nsn = getNamespace(n.name());
	switch(nsn.ns){
		case EXmlNamespace::SVG:
			if(nsn.name == "svg"){
				return this->parseSvgElement(n);
			}else if(nsn.name == "g"){
				return this->parseGElement(n);
			}
			break;
		default:
			//unknown namespace, ignore
			break;
	}
	
	ASSERT(this->namespaces.size() > 0)
	this->namespaces.pop_back();
	ASSERT(this->defaultNamespace.size() > 0)
	this->defaultNamespace.pop_back();
	
	return nullptr;
}

}//~namespace



std::unique_ptr<SvgElement> svgdom::load(const papki::File& f){
	pugi::xml_document doc;
	{
		auto fileContents = f.loadWholeFileIntoMemory();
		if(doc.load_buffer(&*fileContents.begin(), fileContents.size()).status != pugi::xml_parse_status::status_ok){
			TRACE(<< "svgdom::load(): loading XML document failed!" << std::endl)
			return nullptr;
		}
	}
	
	Parser parser;
	
	//return first node which is successfully parsed
	for(auto n = doc.first_child(); !n.empty(); n = n.next_sibling()){
		auto element = parser.parseNode(doc.first_child());
	
		auto ret = std::unique_ptr<SvgElement>(dynamic_cast<SvgElement*>(element.release()));
		if(ret){
			return ret;
		}
	}
	return nullptr;
}



Length Length::parse(const std::string& str) {
	Length ret;

	std::istringstream ss(str);
	
	ss >> std::skipws;
	
	ss >> ret.value;
	
	std::array<char, 3> unit;
	
	ss.width(unit.size() - 1);//for terminating null
	ss.fill(' ');
	
	ss >> (&*unit.begin());
	
	std::string u(&*unit.begin());
	
	if(u.length() == 0){
		ret.unit = EUnit::NUMBER;
	}else if(u == "%"){
		ret.unit = EUnit::PERCENT;
	}else if(u == "em"){
		ret.unit = EUnit::EM;
	}else if(u == "ex"){
		ret.unit = EUnit::EX;
	}else if(u == "px"){
		ret.unit = EUnit::PX;
	}else if(u == "cm"){
		ret.unit = EUnit::CM;
	}else if(u == "in"){
		ret.unit = EUnit::IN;
	}else if(u == "pt"){
		ret.unit = EUnit::PT;
	}else if(u == "pc"){
		ret.unit = EUnit::PC;
	}else{
		ret.unit = EUnit::UNKNOWN;
	}
	
	return ret;
}



std::ostream& operator<<(std::ostream& s, const Length& l){
	s << l.value;
	
	switch(l.unit){
		case EUnit::UNKNOWN:
		case EUnit::NUMBER:
		default:
			break;
		case EUnit::PERCENT:
			s << "%";
			break;
		case EUnit::EM:
			s << "em";
			break;
		case EUnit::EX:
			s << "ex";
			break;
		case EUnit::PX:
			s << "px";
			break;
		case EUnit::CM:
			s << "cm";
			break;
		case EUnit::IN:
			s << "in";
			break;
		case EUnit::PT:
			s << "pt";
			break;
		case EUnit::PC:
			s << "pc";
			break;
	}
	
	return s;
}



void Element::attribsToStream(std::ostream& s) const{
	if(this->id.length() != 0){
		s << " id=\"" << this->id << "\"";
	}
}



void Rectangle::attribsToStream(std::ostream& s)const{
	if(this->x.value != 0){
		s << " x=\"" << this->x << "\"";
	}
	
	if(this->y.value != 0){
		s << " y=\"" << this->y << "\"";
	}
	
	if(this->width.value != 100 || this->width.unit != EUnit::PERCENT){ //if width is not 100% (default value)
		s << " width=\"" << this->width << "\"";
	}
	
	if(this->height.value != 100 || this->height.unit != EUnit::PERCENT){ //if height is not 100% (default value)
		s << " height=\"" << this->height << "\"";
	}
}


namespace{

std::string indentStr(unsigned indent){
	std::string ind;

	std::stringstream ss;
	for(unsigned i = 0; i != indent; ++i){
		ss << "\t";
	}
	return ss.str();
}

}//~namespace


void SvgElement::toStream(std::ostream& s, unsigned indent) const{
	auto ind = indentStr(indent);
	
	s << ind << "<svg";
	this->Element::attribsToStream(s);
	this->Rectangle::attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</svg>";
	}
	s << std::endl;
}

void Container::childrenToStream(std::ostream& s, unsigned indent) const{
	for(auto& e : this->children){
		e->toStream(s, indent);
	}
}

std::string Element::toString() const{
	std::stringstream s;
	this->toStream(s, 0);
	return s.str();
}

void GElement::toStream(std::ostream& s, unsigned indent) const{
	auto ind = indentStr(indent);
	
	s << ind << "<g";
	this->Element::attribsToStream(s);
	
	if(this->children.size() == 0){
		s << "/>";
	}else{
		s << ">" << std::endl;
		this->childrenToStream(s, indent + 1);
		s << ind << "</g>";
	}
	s << std::endl;
}
