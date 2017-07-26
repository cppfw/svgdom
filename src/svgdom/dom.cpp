#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <ratio>

#include <pugixml.hpp>

#include "dom.hpp"
#include "config.hpp"
#include "Exc.hpp"
#include "util.hxx"


using namespace svgdom;


namespace{
Gradient::SpreadMethod_e gradientStringToSpreadMethod(const std::string& str) {
	if(str == "pad"){
		return Gradient::SpreadMethod_e::PAD;
	}else if(str == "reflect"){
		return Gradient::SpreadMethod_e::REFLECT;
	}else if(str == "repeat"){
		return Gradient::SpreadMethod_e::REPEAT;
	}
	return Gradient::SpreadMethod_e::DEFAULT;
}
}


namespace{
enum class XmlNamespace_e{
	UNKNOWN,
	SVG,
	XLINK
};
}

namespace{
const std::string DSvgNamespace = "http://www.w3.org/2000/svg";
const std::string DXlinkNamespace = "http://www.w3.org/1999/xlink";
}

namespace{
struct Parser{
	typedef std::map<std::string, XmlNamespace_e> T_NamespaceMap;
	std::vector<T_NamespaceMap> namespaces;
	
	std::vector<XmlNamespace_e> defaultNamespace;
	
	
	XmlNamespace_e findNamespace(const std::string& ns){
		for(auto i = this->namespaces.rbegin(); i != this->namespaces.rend(); ++i){
			auto iter = i->find(ns);
			if(iter == i->end()){
				continue;
			}
			ASSERT(ns == iter->first)
			return iter->second;
		}
		return XmlNamespace_e::UNKNOWN;
	}
	
	struct NamespaceNamePair{
		XmlNamespace_e ns;
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
				case XmlNamespace_e::SVG:
					if(nsn.name == "id"){
						e.id = a.value();
					}
					break;
				default:
					break;
			}
		}
	}
	
	void fillReferencing(Referencing& e, const pugi::xml_node& n){
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::XLINK:
				case XmlNamespace_e::SVG: //in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
					if(nsn.name == "href"){
						e.iri = a.value();
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
				case XmlNamespace_e::SVG:
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

	void fillViewBoxed(ViewBoxed& v, const pugi::xml_node& n) {
		for (auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
			auto nsn = this->getNamespace(a.name());
			switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "viewBox") {
					v.viewBox = SvgElement::parseViewbox(a.value());
				}
				else if (nsn.name == "preserveAspectRatio") {
					v.parseAndFillPreserveAspectRatio(a.value());
				}
				break;
			default:
				break;
			}
		}
	}
	
	void fillContainer(Container& c, const pugi::xml_node& n){
		this->fillElement(c, n);
		
		ASSERT(c.children.size() == 0)
		for(auto i = n.first_child(); !i.empty(); i = i.next_sibling()){
			if(auto res = this->parseNode(i)){
				c.children.push_back(std::move(res));
				c.children.back()->parent = &c;
			}
		}
	}
	
	void fillTransformable(Transformable& t, const pugi::xml_node& n){
		ASSERT(t.transformations.size() == 0)
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "transform"){
						t.transformations = Transformable::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
	}
	
	void fillStyleable(Styleable& s, const pugi::xml_node& n){
		ASSERT(s.styles.size() == 0)
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "style"){
						s.styles = Styleable::parse(a.value());
						break;
					}
					{
						StyleProperty_e type = Styleable::stringToProperty(nsn.name);
						if(type != StyleProperty_e::UNKNOWN){
							s.styles[type] = Styleable::parseStylePropertyValue(type, a.value());
						}
					}
					break;
				default:
					break;
			}
		}
	}
	
	void fillGradient(Gradient& g, const pugi::xml_node& n){
		this->fillContainer(g, n);
		this->fillReferencing(g, n);
		this->fillStyleable(g, n);
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "spreadMethod"){
						g.spreadMethod = gradientStringToSpreadMethod(a.value());
					}else if(nsn.name == "gradientTransform"){
						g.transformations = Transformable::parse(a.value());
					}else if(nsn.name == "gradientUnits"){
						if(std::string("userSpaceOnUse") == a.value()){
							g.units = Gradient::Units_e::USER_SPACE_ON_USE;
						}else if(std::string("objectBoundingBox") == a.value()){
							g.units = Gradient::Units_e::OBJECT_BOUNDING_BOX;
						}
					}
					break;
				default:
					break;
			}
		}
	}
	
	void fillShape(Shape& s, const pugi::xml_node& n){
		this->fillElement(s, n);
		this->fillStyleable(s, n);
		this->fillTransformable(s, n);
	}
	
	std::unique_ptr<Gradient::StopElement> parseGradientStopElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "stop")
		
		auto ret = utki::makeUnique<Gradient::StopElement>();
		this->fillStyleable(*ret, n);
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "offset"){
						std::istringstream s(a.value());
						s >> ret->offset;
						if(!s.eof() && s.peek() == '%'){
							ret->offset /= 100;
						}
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<SvgElement> parseSvgElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "svg")
		
		auto ret = utki::makeUnique<SvgElement>();
		
		this->fillStyleable(*ret, n);
		this->fillRectangle(*ret, n);
		this->fillContainer(*ret, n);
		this->fillViewBoxed(*ret, n);
		
		return ret;
	}

	std::unique_ptr<SymbolElement> parseSymbolElement(const pugi::xml_node& n) {
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "symbol")

//		TRACE(<< "parseSymbolElement():" << std::endl)

		auto ret = utki::makeUnique<SymbolElement>();

		this->fillStyleable(*ret, n);
		this->fillContainer(*ret, n);
		this->fillViewBoxed(*ret, n);

		return ret;
	}
	
	std::unique_ptr<GElement> parseGElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "g")
		
		auto ret = utki::makeUnique<GElement>();
		
		this->fillTransformable(*ret, n);
		this->fillStyleable(*ret, n);
		this->fillContainer(*ret, n);
		
		return ret;
	}
	
	std::unique_ptr<DefsElement> parseDefsElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "defs")
		
		auto ret = utki::makeUnique<DefsElement>();
		
		this->fillTransformable(*ret, n);
		this->fillStyleable(*ret, n);
		this->fillContainer(*ret, n);
		
		return ret;
	}

	std::unique_ptr<UseElement> parseUseElement(const pugi::xml_node& n) {
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "use")

		auto ret = utki::makeUnique<UseElement>();

		this->fillElement(*ret, n);
		this->fillTransformable(*ret, n);
		this->fillStyleable(*ret, n);
		this->fillReferencing(*ret, n);
		this->fillRectangle(*ret, n);

		return ret;
	}
	
	std::unique_ptr<PathElement> parsePathElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "path")
		
		auto ret = utki::makeUnique<PathElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "d"){
						ret->path = PathElement::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<RectElement> parseRectElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "rect")
		
		auto ret = utki::makeUnique<RectElement>();
		
		this->fillShape(*ret, n);
		this->fillRectangle(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "rx"){
						ret->rx = Length::parse(a.value());
					}else if(nsn.name == "ry"){
						ret->ry = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<CircleElement> parseCircleElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "circle")
		
		auto ret = utki::makeUnique<CircleElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "cx"){
						ret->cx = Length::parse(a.value());
					}else if(nsn.name == "cy"){
						ret->cy = Length::parse(a.value());
					}else if(nsn.name == "r"){
						ret->r = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<LineElement> parseLineElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "line")
		
		auto ret = utki::makeUnique<LineElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "x1"){
						ret->x1 = Length::parse(a.value());
					}else if(nsn.name == "y1"){
						ret->y1 = Length::parse(a.value());
					}else if(nsn.name == "x2"){
						ret->x2 = Length::parse(a.value());
					}else if(nsn.name == "y2"){
						ret->y2 = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<PolylineElement> parsePolylineElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "polyline")
		
		auto ret = utki::makeUnique<PolylineElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "points"){
						ret->points = ret->parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<PolygonElement> parsePolygonElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "polygon")
		
		auto ret = utki::makeUnique<PolygonElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "points"){
						ret->points = ret->parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<EllipseElement> parseEllipseElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "ellipse")
		
		auto ret = utki::makeUnique<EllipseElement>();
		
		this->fillShape(*ret, n);

		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "cx"){
						ret->cx = Length::parse(a.value());
					}else if(nsn.name == "cy"){
						ret->cy = Length::parse(a.value());
					}else if(nsn.name == "rx"){
						ret->rx = Length::parse(a.value());
					}else if(nsn.name == "ry"){
						ret->ry = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<LinearGradientElement> parseLinearGradientElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "linearGradient")
		
		auto ret = utki::makeUnique<LinearGradientElement>();
		
		this->fillGradient(*ret, n);
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "x1"){
						ret->x1 = Length::parse(a.value());
					}else if(nsn.name == "y1"){
						ret->y1 = Length::parse(a.value());
					}else if(nsn.name == "x2"){
						ret->x2 = Length::parse(a.value());
					}else if(nsn.name == "y2"){
						ret->y2 = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
	
	std::unique_ptr<RadialGradientElement> parseRadialGradientElement(const pugi::xml_node& n){
		ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
		ASSERT(getNamespace(n.name()).name == "radialGradient")
		
		auto ret = utki::makeUnique<RadialGradientElement>();
		
		this->fillGradient(*ret, n);
		
		for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()){
			auto nsn = this->getNamespace(a.name());
			switch(nsn.ns){
				case XmlNamespace_e::SVG:
					if(nsn.name == "cx"){
						ret->cx = Length::parse(a.value());
					}else if(nsn.name == "cy"){
						ret->cy = Length::parse(a.value());
					}else if(nsn.name == "r"){
						ret->r = Length::parse(a.value());
					}else if(nsn.name == "fx"){
						ret->fx = Length::parse(a.value());
					}else if(nsn.name == "fy"){
						ret->fy = Length::parse(a.value());
					}
					break;
				default:
					break;
			}
		}
		
		return ret;
	}
};//~class
}

namespace{
std::unique_ptr<svgdom::Element> Parser::parseNode(const pugi::xml_node& n){
	//parse default namespace
	{
		pugi::xml_attribute dn = n.attribute("xmlns");
		if(!dn.empty()){
			if(std::string(dn.value()) == DSvgNamespace){
				this->defaultNamespace.push_back(XmlNamespace_e::SVG);
			}else{
				this->defaultNamespace.push_back(XmlNamespace_e::UNKNOWN);
			}
		}else{
			if(this->defaultNamespace.size() == 0){
				this->defaultNamespace.push_back(XmlNamespace_e::UNKNOWN);
			}else{
				this->defaultNamespace.push_back(this->defaultNamespace.back());
			}
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
			
			if(DSvgNamespace == a.value()){
				this->namespaces.back()[ns] = XmlNamespace_e::SVG;
			}else if(DXlinkNamespace == a.value()){
				this->namespaces.back()[ns] = XmlNamespace_e::XLINK;
			}
		}
	}
	
	utki::ScopeExit scopeExit([this](){
		ASSERT(this->namespaces.size() > 0)
		this->namespaces.pop_back();
		ASSERT(this->defaultNamespace.size() > 0)
		this->defaultNamespace.pop_back();
	});
	
	auto nsn = getNamespace(n.name());
//	TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch(nsn.ns){
		case XmlNamespace_e::SVG:
			if(nsn.name == "svg"){
				return this->parseSvgElement(n);
			}else if(nsn.name == "symbol") {
				return this->parseSymbolElement(n);
			}else if(nsn.name == "g"){
				return this->parseGElement(n);
			}else if(nsn.name == "defs"){
				return this->parseDefsElement(n);
			}else if (nsn.name == "use") {
				return this->parseUseElement(n);
			}else if(nsn.name == "path"){
				return this->parsePathElement(n);
			}else if(nsn.name == "linearGradient"){
				return this->parseLinearGradientElement(n);
			}else if(nsn.name == "radialGradient"){
				return this->parseRadialGradientElement(n);
			}else if(nsn.name == "stop"){
				return this->parseGradientStopElement(n);
			}else if(nsn.name == "rect"){
				return this->parseRectElement(n);
			}else if(nsn.name == "circle"){
				return this->parseCircleElement(n);
			}else if(nsn.name == "ellipse"){
				return this->parseEllipseElement(n);
			}else if(nsn.name == "line"){
				return this->parseLineElement(n);
			}else if(nsn.name == "polyline"){
				return this->parsePolylineElement(n);
			}else if(nsn.name == "polygon"){
				return this->parsePolygonElement(n);
			}
			
			break;
		default:
			//unknown namespace, ignore
			break;
	}
	
	return nullptr;
}
}

namespace{
void resolveReferences(Element& e, SvgElement& root){
	if(auto r = dynamic_cast<Referencing*>(&e)){
		if(r->iri.length() != 0 && r->iri[0] == '#'){
			r->ref = root.findById(r->iri.substr(1, r->iri.length() - 1));
		}
	}
	
	if(auto s = dynamic_cast<Styleable*>(&e)){
		for(auto& p : s->styles){
			if(p.second.isUrl()){
				if(p.second.str.length() != 0 && p.second.str[0] == '#'){
					p.second.url = root.findById(p.second.str.substr(1, p.second.str.length() - 1));
				}
			}
		}
	}
	
	if(auto container = dynamic_cast<Container*>(&e)){
		for(auto& c : container->children){
			resolveReferences(*c, root);
		}
	}
}
}

namespace{
std::unique_ptr<SvgElement> load(const pugi::xml_document& doc){
	Parser parser;
	
	//return first node which is successfully parsed
	for(auto n = doc.first_child(); !n.empty(); n = n.next_sibling()){
		auto element = parser.parseNode(doc.first_child());

		if(auto svgElement = dynamic_cast<SvgElement*>(element.get())){
			auto ret = std::unique_ptr<SvgElement>(svgElement);
			element.release();
			resolveReferences(*ret, *ret);
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


