#include "Parser.hxx"

#include <utki/debug.hpp>
#include <utki/util.hpp>

using namespace svgdom;

namespace{
const std::string DSvgNamespace = "http://www.w3.org/2000/svg";
const std::string DXlinkNamespace = "http://www.w3.org/1999/xlink";
}

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
		
		this->namespaces.push_back(decltype(this->namespaces)::value_type());
		
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

Parser::XmlNamespace_e Parser::findNamespace(const std::string& ns) {
	for(auto i = this->namespaces.rbegin(); i != this->namespaces.rend(); ++i) {
		auto iter = i->find(ns);
		if (iter == i->end()) {
			continue;
		}
		ASSERT(ns == iter->first)
		return iter->second;
	}
	return XmlNamespace_e::UNKNOWN;
}

Parser::NamespaceNamePair Parser::getNamespace(const std::string& xmlAttributeName) {
	NamespaceNamePair ret;

	auto colonIndex = xmlAttributeName.find_first_of(':');
	if (colonIndex == std::string::npos) {
		ret.ns = this->defaultNamespace.back();
		ret.name = xmlAttributeName;
		return ret;
	}

	ASSERT(xmlAttributeName.length() >= colonIndex + 1)

	ret.ns = this->findNamespace(xmlAttributeName.substr(0, colonIndex));
	ret.name = xmlAttributeName.substr(colonIndex + 1, xmlAttributeName.length() - 1 - colonIndex);

	return ret;
}

void Parser::fillElement(Element& e, const pugi::xml_node& n) {
	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "id") {
					e.id = a.value();
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillContainer(Container& c, const pugi::xml_node& n) {
	this->fillElement(c, n);

	ASSERT(c.children.size() == 0)
	for(auto i = n.first_child(); !i.empty(); i = i.next_sibling()) {
		if (auto res = this->parseNode(i)) {
			c.children.push_back(std::move(res));
			c.children.back()->parent = &c;
		}
	}
}

void Parser::fillGradient(Gradient& g, const pugi::xml_node& n) {
	this->fillContainer(g, n);
	this->fillReferencing(g, n);
	this->fillStyleable(g, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "spreadMethod") {
					g.spreadMethod = gradientStringToSpreadMethod(a.value());
				} else if (nsn.name == "gradientTransform") {
					g.transformations = Transformable::parse(a.value());
				} else if (nsn.name == "gradientUnits") {
					if (std::string("userSpaceOnUse") == a.value()) {
						g.units = Gradient::Units_e::USER_SPACE_ON_USE;
					} else if (std::string("objectBoundingBox") == a.value()) {
						g.units = Gradient::Units_e::OBJECT_BOUNDING_BOX;
					}
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillRectangle(Rectangle& r, const pugi::xml_node& n) {
	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "x") {
					r.x = Length::parse(a.value());
				} else if (nsn.name == "y") {
					r.y = Length::parse(a.value());
				} else if (nsn.name == "width") {
					r.width = Length::parse(a.value());
				} else if (nsn.name == "height") {
					r.height = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillReferencing(Referencing& e, const pugi::xml_node& n) {
	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::XLINK:
			case XmlNamespace_e::SVG: //in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
				if (nsn.name == "href") {
					e.iri = a.value();
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillShape(Shape& s, const pugi::xml_node& n) {
	this->fillElement(s, n);
	this->fillStyleable(s, n);
	this->fillTransformable(s, n);
}

void Parser::fillStyleable(Styleable& s, const pugi::xml_node& n) {
	ASSERT(s.styles.size() == 0)

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "style") {
					s.styles = Styleable::parse(a.value());
					break;
				}
			{
				StyleProperty_e type = Styleable::stringToProperty(nsn.name);
				if (type != StyleProperty_e::UNKNOWN) {
					s.styles[type] = Styleable::parseStylePropertyValue(type, a.value());
				}
			}
				break;
			default:
				break;
		}
	}
}

void Parser::fillTransformable(Transformable& t, const pugi::xml_node& n) {
	ASSERT(t.transformations.size() == 0)

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "transform") {
					t.transformations = Transformable::parse(a.value());
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillViewBoxed(ViewBoxed& v, const pugi::xml_node& n) {
	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "viewBox") {
					v.viewBox = SvgElement::parseViewbox(a.value());
				} else if (nsn.name == "preserveAspectRatio") {
					v.parseAndFillPreserveAspectRatio(a.value());
				}
				break;
			default:
				break;
		}
	}
}

std::unique_ptr<CircleElement> Parser::parseCircleElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "circle")

	auto ret = utki::makeUnique<CircleElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "cx") {
					ret->cx = Length::parse(a.value());
				} else if (nsn.name == "cy") {
					ret->cy = Length::parse(a.value());
				} else if (nsn.name == "r") {
					ret->r = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<DefsElement> Parser::parseDefsElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "defs")

	auto ret = utki::makeUnique<DefsElement>();

	this->fillTransformable(*ret, n);
	this->fillStyleable(*ret, n);
	this->fillContainer(*ret, n);

	return ret;
}

std::unique_ptr<EllipseElement> Parser::parseEllipseElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "ellipse")

	auto ret = utki::makeUnique<EllipseElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "cx") {
					ret->cx = Length::parse(a.value());
				} else if (nsn.name == "cy") {
					ret->cy = Length::parse(a.value());
				} else if (nsn.name == "rx") {
					ret->rx = Length::parse(a.value());
				} else if (nsn.name == "ry") {
					ret->ry = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<GElement> Parser::parseGElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "g")

	auto ret = utki::makeUnique<GElement>();

	this->fillTransformable(*ret, n);
	this->fillStyleable(*ret, n);
	this->fillContainer(*ret, n);

	return ret;
}

std::unique_ptr<Gradient::StopElement> Parser::parseGradientStopElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "stop")

	auto ret = utki::makeUnique<Gradient::StopElement>();
	this->fillStyleable(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "offset") {
					std::istringstream s(a.value());
					s >> ret->offset;
					if (!s.eof() && s.peek() == '%') {
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

std::unique_ptr<LineElement> Parser::parseLineElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "line")

	auto ret = utki::makeUnique<LineElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "x1") {
					ret->x1 = Length::parse(a.value());
				} else if (nsn.name == "y1") {
					ret->y1 = Length::parse(a.value());
				} else if (nsn.name == "x2") {
					ret->x2 = Length::parse(a.value());
				} else if (nsn.name == "y2") {
					ret->y2 = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<LinearGradientElement> Parser::parseLinearGradientElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "linearGradient")

	auto ret = utki::makeUnique<LinearGradientElement>();

	this->fillGradient(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "x1") {
					ret->x1 = Length::parse(a.value());
				} else if (nsn.name == "y1") {
					ret->y1 = Length::parse(a.value());
				} else if (nsn.name == "x2") {
					ret->x2 = Length::parse(a.value());
				} else if (nsn.name == "y2") {
					ret->y2 = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<PathElement> Parser::parsePathElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "path")

	auto ret = utki::makeUnique<PathElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "d") {
					ret->path = PathElement::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<PolygonElement> Parser::parsePolygonElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "polygon")

	auto ret = utki::makeUnique<PolygonElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "points") {
					ret->points = ret->parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<PolylineElement> Parser::parsePolylineElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "polyline")

	auto ret = utki::makeUnique<PolylineElement>();

	this->fillShape(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "points") {
					ret->points = ret->parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<RadialGradientElement> Parser::parseRadialGradientElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "radialGradient")

	auto ret = utki::makeUnique<RadialGradientElement>();

	this->fillGradient(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "cx") {
					ret->cx = Length::parse(a.value());
				} else if (nsn.name == "cy") {
					ret->cy = Length::parse(a.value());
				} else if (nsn.name == "r") {
					ret->r = Length::parse(a.value());
				} else if (nsn.name == "fx") {
					ret->fx = Length::parse(a.value());
				} else if (nsn.name == "fy") {
					ret->fy = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<RectElement> Parser::parseRectElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "rect")

	auto ret = utki::makeUnique<RectElement>();

	this->fillShape(*ret, n);
	this->fillRectangle(*ret, n);

	for(auto a = n.first_attribute(); !a.empty(); a = a.next_attribute()) {
		auto nsn = this->getNamespace(a.name());
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "rx") {
					ret->rx = Length::parse(a.value());
				} else if (nsn.name == "ry") {
					ret->ry = Length::parse(a.value());
				}
				break;
			default:
				break;
		}
	}

	return ret;
}

std::unique_ptr<SvgElement> Parser::parseSvgElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "svg")

	auto ret = utki::makeUnique<SvgElement>();

	this->fillStyleable(*ret, n);
	this->fillRectangle(*ret, n);
	this->fillContainer(*ret, n);
	this->fillViewBoxed(*ret, n);

	return ret;
}

std::unique_ptr<SymbolElement> Parser::parseSymbolElement(const pugi::xml_node& n) {
	ASSERT(getNamespace(n.name()).ns == XmlNamespace_e::SVG)
	ASSERT(getNamespace(n.name()).name == "symbol")

	//		TRACE(<< "parseSymbolElement():" << std::endl)

	auto ret = utki::makeUnique<SymbolElement>();

	this->fillStyleable(*ret, n);
	this->fillContainer(*ret, n);
	this->fillViewBoxed(*ret, n);

	return ret;
}

std::unique_ptr<UseElement> Parser::parseUseElement(const pugi::xml_node& n) {
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
