#include "Parser.hxx"
#include "util.hxx"

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

void Parser::parseNode(){
	//parse default namespace
	{
		auto i = this->attributes.find("xmlns");
		if(i != this->attributes.end()){
			if(i->second == DSvgNamespace){
				this->defaultNamespaceStack.push_back(XmlNamespace_e::SVG);
				this->svgNsPrefix.clear();
			}else if(i->second == DXlinkNamespace){
				this->defaultNamespaceStack.push_back(XmlNamespace_e::XLINK);
				this->xlinkNsPrefix.clear();
			}else{
				this->defaultNamespaceStack.push_back(XmlNamespace_e::UNKNOWN);
			}
		}else{
			if(this->defaultNamespaceStack.size() == 0){
				this->defaultNamespaceStack.push_back(XmlNamespace_e::UNKNOWN);
			}else{
				this->defaultNamespaceStack.push_back(this->defaultNamespaceStack.back());
			}
		}
	}
	
	//parse other namespaces
	{
		std::string xmlns = "xmlns:";
		
		this->namespacesStack.push_back(decltype(this->namespacesStack)::value_type());
		
		for(auto& e : this->attributes){
			const auto& attr = e.first;
			
			if(attr.substr(0, xmlns.length()) != xmlns){
				continue;
			}
			
			ASSERT(attr.length() >= xmlns.length())
			auto ns = attr.substr(xmlns.length(), attr.length() - xmlns.length());
			
			if(e.second == DSvgNamespace){
				this->namespacesStack.back()[ns] = XmlNamespace_e::SVG;
				this->svgNsPrefix = std::string(":") + ns;
			}else if(e.second == DXlinkNamespace){
				this->namespacesStack.back()[ns] = XmlNamespace_e::XLINK;
				this->xlinkNsPrefix = std::string(":") + ns;
			}
		}
	}
	
	utki::ScopeExit scopeExit([this](){
		ASSERT(this->namespacesStack.size() > 0)
		this->namespacesStack.pop_back();
		ASSERT(this->defaultNamespaceStack.size() > 0)
		this->defaultNamespaceStack.pop_back();
	});
	
	auto nsn = getNamespace(this->element);
//	TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch(nsn.ns){
		case XmlNamespace_e::SVG:
			if(nsn.name == "svg"){
				this->parseSvgElement();
			}else if(nsn.name == "symbol") {
				this->parseSymbolElement();
			}else if(nsn.name == "g"){
				this->parseGElement();
			}else if(nsn.name == "defs"){
				this->parseDefsElement();
			}else if (nsn.name == "use") {
				this->parseUseElement();
			}else if(nsn.name == "path"){
				this->parsePathElement();
			}else if(nsn.name == "linearGradient"){
				this->parseLinearGradientElement();
			}else if(nsn.name == "radialGradient"){
				this->parseRadialGradientElement();
			}else if(nsn.name == "stop"){
				this->parseGradientStopElement();
			}else if(nsn.name == "rect"){
				this->parseRectElement();
			}else if(nsn.name == "circle"){
				this->parseCircleElement();
			}else if(nsn.name == "ellipse"){
				this->parseEllipseElement();
			}else if(nsn.name == "line"){
				this->parseLineElement();
			}else if(nsn.name == "polyline"){
				this->parsePolylineElement();
			}else if(nsn.name == "polygon"){
				this->parsePolygonElement();
			}else if(nsn.name == "filter"){
				this->parseFilterElement();
			}else if(nsn.name == "feGaussianBlur"){
				this->parseFeGaussianBlurElement();
			}else if(nsn.name == "image"){
				this->parseImageElement();
			}
			
			break;
		default:
			//unknown namespace, ignore
			break;
	}
}

Parser::XmlNamespace_e Parser::findNamespace(const std::string& ns) {
	for(auto i = this->namespacesStack.rbegin(); i != this->namespacesStack.rend(); ++i) {
		auto iter = i->find(ns);
		if (iter == i->end()) {
			continue;
		}
		ASSERT(ns == iter->first)
		return iter->second;
	}
	return XmlNamespace_e::UNKNOWN;
}

Parser::NamespaceNamePair Parser::getNamespace(const std::string& xmlName) {
	NamespaceNamePair ret;

	auto colonIndex = xmlName.find_first_of(':');
	if (colonIndex == std::string::npos) {
		ret.ns = this->defaultNamespaceStack.back();
		ret.name = xmlName;
		return ret;
	}

	ASSERT(xmlName.length() >= colonIndex + 1)

	ret.ns = this->findNamespace(xmlName.substr(0, colonIndex));
	ret.name = xmlName.substr(colonIndex + 1, xmlName.length() - 1 - colonIndex);

	return ret;
}

void Parser::fillElement(Element& e) {
	auto i = this->attributes.find(this->svgNsPrefix + "id");
	if(i != this->attributes.end()){
		e.id = i->second;
	}
}

void Parser::fillGradient(Gradient& g) {
	this->fillElement(g);
	this->fillReferencing(g);
	this->fillStyleable(g);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "spreadMethod");
		if(i != this->attributes.end()){
			g.spreadMethod = gradientStringToSpreadMethod(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "gradientTransform");
		if(i != this->attributes.end()){
			g.transformations = Transformable::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "gradientUnits");
		if(i != this->attributes.end()){
			g.units = parseCoordinateUnits(i->second);
		}
	}
}

void Parser::fillRectangle(Rectangle& r) {
	{
		auto i = this->attributes.find(this->svgNsPrefix + "x");
		if(i != this->attributes.end()){
			r.x = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "y");
		if(i != this->attributes.end()){
			r.y = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "width");
		if(i != this->attributes.end()){
			r.width = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "height");
		if(i != this->attributes.end()){
			r.height = Length::parse(i->second);
		}
	}
}

void Parser::fillReferencing(Referencing& e) {
	{
		auto i = this->attributes.find(this->xlinkNsPrefix + "href");
		if(i == this->attributes.end()){
			i = this->attributes.find(this->svgNsPrefix + "href");//in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
		}
		if(i != this->attributes.end()){
			e.iri = i->second;
		}
	}
}

void Parser::fillShape(Shape& s) {
	this->fillElement(s);
	this->fillStyleable(s);
	this->fillTransformable(s);
}

void Parser::fillStyleable(Styleable& s) {
	ASSERT(s.styles.size() == 0)
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "style");
		if(i != this->attributes.end()){
			s.styles = Styleable::parse(i->second);
		}
	}
	
	for(auto type = StyleProperty_e::UNKNOWN; type != StyleProperty_e::ENUM_SIZE; type = StyleProperty_e(unsigned(type) + 1)){
		auto name = Styleable::propertyToString(type);
		if(name.length() == 0){
			continue;
		}
		auto i = this->attributes.find(this->svgNsPrefix + name);
		if(i != this->attributes.end()){
			s.styles[type] = Styleable::parseStylePropertyValue(type, i->second);
		}
	}
}

void Parser::fillTransformable(Transformable& t) {
	ASSERT(t.transformations.size() == 0)
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "transform");
		if(i != this->attributes.end()){
			t.transformations = Transformable::parse(i->second);
		}
	}
}

void Parser::fillViewBoxed(ViewBoxed& v) {
	{
		auto i = this->attributes.find(this->svgNsPrefix + "viewBox");
		if(i != this->attributes.end()){
			v.viewBox = SvgElement::parseViewbox(i->second);
		}
	}
}

void Parser::fillAspectRatioed(AspectRatioed& e) {
	{
		auto i = this->attributes.find(this->svgNsPrefix + "preserveAspectRatio");
		if(i != this->attributes.end()){
			e.parseAndFillPreserveAspectRatio(i->second);
		}
	}
}

void Parser::addElement(std::unique_ptr<Element> e) {
	ASSERT(this->parentStack.back())
	this->parentStack.back()->children.push_back(std::move(e));
	this->parentStack.push_back(nullptr);
}

void Parser::addElement(std::unique_ptr<Element> e, Container* c) {
	ASSERT(e)
	ASSERT(c)
	ASSERT(this->parentStack.back())
	this->parentStack.back()->children.push_back(std::move(e));
	this->parentStack.push_back(c);
}



void Parser::parseCircleElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "circle")

	auto ret = utki::makeUnique<CircleElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "cx");
		if(i != this->attributes.end()){
			ret->cx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "cy");
		if(i != this->attributes.end()){
			ret->cy = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "r");
		if(i != this->attributes.end()){
			ret->r = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseDefsElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "defs")

	auto ret = utki::makeUnique<DefsElement>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	this->addElement(std::move(ret), ret.get());
}

void Parser::parseEllipseElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "ellipse")

	auto ret = utki::makeUnique<EllipseElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "cx");
		if(i != this->attributes.end()){
			ret->cx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "cy");
		if(i != this->attributes.end()){
			ret->cy = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "rx");
		if(i != this->attributes.end()){
			ret->rx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "ry");
		if(i != this->attributes.end()){
			ret->ry = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseGElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "g")

	auto ret = utki::makeUnique<GElement>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	this->addElement(std::move(ret), ret.get());
}

void Parser::parseGradientStopElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "stop")

	auto ret = utki::makeUnique<Gradient::StopElement>();
	
	this->fillStyleable(*ret);
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "offset");
		if(i != this->attributes.end()){
			std::istringstream s(i->second);
			s >> ret->offset;
			if (!s.eof() && s.peek() == '%') {
				ret->offset /= 100;
			}
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseLineElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "line")

	auto ret = utki::makeUnique<LineElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "x1");
		if(i != this->attributes.end()){
			ret->x1 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "y1");
		if(i != this->attributes.end()){
			ret->y1 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "x2");
		if(i != this->attributes.end()){
			ret->x2 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "y2");
		if(i != this->attributes.end()){
			ret->y2 = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseFilterElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "filter")
	
	auto ret = utki::makeUnique<FilterElement>();
	
	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(*ret);
	this->fillReferencing(*ret);
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "filterUnits");
		if(i != this->attributes.end()){
			ret->filterUnits = svgdom::parseCoordinateUnits(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "primitiveUnits");
		if(i != this->attributes.end()){
			ret->primitiveUnits = svgdom::parseCoordinateUnits(i->second);
		}
	}
	
	this->addElement(std::move(ret), ret.get());
}

void Parser::fillFilterPrimitive(FilterPrimitive& p) {
	this->fillElement(p);
	this->fillRectangle(p);
	this->fillStyleable(p);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "result");
		if(i != this->attributes.end()){
			p.result = i->second;
		}
	}
}

void Parser::fillInputableFilterPrimitive(InputableFilterPrimitive& p) {
	this->fillFilterPrimitive(p);
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "in");
		if(i != this->attributes.end()){
			p.in = i->second;
		}
	}
}


void Parser::parseFeGaussianBlurElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "feGaussianBlur")
	
	auto ret = utki::makeUnique<FeGaussianBlurElement>();
	
	this->fillInputableFilterPrimitive(*ret);
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "stdDeviation");
		if(i != this->attributes.end()){
			ret->stdDeviation = parseNumberOptionalNumber(i->second, {{-1, -1}});
		}
	}
	
	this->addElement(std::move(ret));
}



void Parser::parseLinearGradientElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "linearGradient")

	auto ret = utki::makeUnique<LinearGradientElement>();

	this->fillGradient(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "x1");
		if(i != this->attributes.end()){
			ret->x1 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "y1");
		if(i != this->attributes.end()){
			ret->y1 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "x2");
		if(i != this->attributes.end()){
			ret->x2 = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "y2");
		if(i != this->attributes.end()){
			ret->y2 = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret), ret.get());
}

void Parser::parsePathElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "path")

	auto ret = utki::makeUnique<PathElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "d");
		if(i != this->attributes.end()){
			ret->path = PathElement::parse(i->second);
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolygonElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "polygon")

	auto ret = utki::makeUnique<PolygonElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "points");
		if(i != this->attributes.end()){
			ret->points = ret->parse(i->second);
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolylineElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "polyline")

	auto ret = utki::makeUnique<PolylineElement>();

	this->fillShape(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "points");
		if(i != this->attributes.end()){
			ret->points = ret->parse(i->second);
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseRadialGradientElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "radialGradient")

	auto ret = utki::makeUnique<RadialGradientElement>();

	this->fillGradient(*ret);
	
	{
		auto i = this->attributes.find(this->svgNsPrefix + "cx");
		if(i != this->attributes.end()){
			ret->cx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "cy");
		if(i != this->attributes.end()){
			ret->cy = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "r");
		if(i != this->attributes.end()){
			ret->r = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "fx");
		if(i != this->attributes.end()){
			ret->fx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "fy");
		if(i != this->attributes.end()){
			ret->fy = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret), ret.get());
}

void Parser::parseRectElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "rect")

	auto ret = utki::makeUnique<RectElement>();

	this->fillShape(*ret);
	this->fillRectangle(*ret);

	{
		auto i = this->attributes.find(this->svgNsPrefix + "rx");
		if(i != this->attributes.end()){
			ret->rx = Length::parse(i->second);
		}
	}
	{
		auto i = this->attributes.find(this->svgNsPrefix + "ry");
		if(i != this->attributes.end()){
			ret->ry = Length::parse(i->second);
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseSvgElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "svg")

	auto ret = utki::makeUnique<SvgElement>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(*ret);
	this->fillViewBoxed(*ret);
	this->fillAspectRatioed(*ret);

	if(!this->svg){
		this->svg = ret.get();
	}
	
	this->addElement(std::move(ret), ret.get());
}

void Parser::parseImageElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "image")

	auto ret = utki::makeUnique<ImageElement>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillTransformable(*ret);
	this->fillRectangle(*ret);
	this->fillReferencing(*ret);
	this->fillAspectRatioed(*ret);

	this->addElement(std::move(ret));
}


void Parser::parseSymbolElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "symbol")

	//		TRACE(<< "parseSymbolElement():" << std::endl)

	auto ret = utki::makeUnique<SymbolElement>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillViewBoxed(*ret);
	this->fillAspectRatioed(*ret);

	this->addElement(std::move(ret), ret.get());
}

void Parser::parseUseElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "use")

	auto ret = utki::makeUnique<UseElement>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);
	this->fillReferencing(*ret);
	this->fillRectangle(*ret);

	this->addElement(std::move(ret));
}

void Parser::onElementStart(const utki::Buf<char> name) {
	this->element = utki::toString(name);
}

void Parser::onElementEnd(const utki::Buf<char> name) {
	this->parentStack.pop_back();
}

void Parser::onAttributeParsed(const utki::Buf<char> name, const utki::Buf<char> value) {
	ASSERT(this->element.length() != 0)
	this->attributes[utki::toString(name)] = utki::toString(value);
}

void Parser::onAttributesEnd(bool isEmptyElement) {
	if(this->parentStack.back()){
		this->parseNode();
	}
	this->attributes.clear();
	this->element.clear();
}

void Parser::onContentParsed(const utki::Buf<char> str) {
	//do nothing for now
}

std::unique_ptr<SvgElement> Parser::getDom() {
	if(!this->svg){
		return nullptr;
	}
	ASSERT(this->root.children.size() != 0)
	
	this->root.children.front().release();
	this->root.children.clear();
	return std::unique_ptr<SvgElement>(this->svg);
}
