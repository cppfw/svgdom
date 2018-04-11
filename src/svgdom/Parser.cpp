#include "Parser.hxx"
#include "util.hxx"

#include "Exc.hpp"

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

void Parser::pushNamespaces() {
	//parse default namespace
	{
		auto i = this->attributes.find("xmlns");
		if(i != this->attributes.end()){
			if(i->second == DSvgNamespace){
				this->defaultNamespaceStack.push_back(XmlNamespace_e::SVG);
			}else if(i->second == DXlinkNamespace){
				this->defaultNamespaceStack.push_back(XmlNamespace_e::XLINK);
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
			auto nsName = attr.substr(xmlns.length(), attr.length() - xmlns.length());
			
			if(e.second == DSvgNamespace){
				this->namespacesStack.back()[nsName] = XmlNamespace_e::SVG;
			}else if(e.second == DXlinkNamespace){
				this->namespacesStack.back()[nsName] = XmlNamespace_e::XLINK;
			}
		}
		
		this->flippedNamespacesStack.push_back(utki::flipMap(this->namespacesStack.back()));
	}
}

void Parser::popNamespaces() {
	ASSERT(this->namespacesStack.size() != 0)
	this->namespacesStack.pop_back();
	ASSERT(this->defaultNamespaceStack.size() != 0)
	this->defaultNamespaceStack.pop_back();
	ASSERT(this->flippedNamespacesStack.size() != 0)
	this->flippedNamespacesStack.pop_back();
}

void Parser::parseNode(){
	auto nsn = this->getNamespace(this->element);
//	TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch(nsn.ns){
		case XmlNamespace_e::SVG:
			//TODO: optimize using map?
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
			}else if(nsn.name == "feColorMatrix"){
				this->parseFeColorMatrixElement();
			}else if(nsn.name == "feBlend"){
				this->parseFeBlendElement();
			}else if(nsn.name == "feComposite"){
				this->parseFeCompositeElement();
			}else if(nsn.name == "image"){
				this->parseImageElement();
			}else if(nsn.name == "mask"){
				this->parseMaskElement();
			}else if(nsn.name == "text"){
				this->parseTextElement();
			}else{
				break;
			}
			return;
		default:
			//unknown namespace, ignore
			break;
	}
	this->parentStack.push_back(nullptr);
}

Parser::XmlNamespace_e Parser::findNamespace(const std::string& ns) {
	for(auto i = this->namespacesStack.rbegin(), e = this->namespacesStack.rend(); i != e; ++i) {
		auto iter = i->find(ns);
		if (iter == i->end()) {
			continue;
		}
		ASSERT(ns == iter->first)
		return iter->second;
	}
	return XmlNamespace_e::UNKNOWN;
}

const std::string* Parser::findFlippedNamespace(XmlNamespace_e ns) {
	for(auto i = this->flippedNamespacesStack.rbegin(), e = this->flippedNamespacesStack.rend(); i != e; ++i){
		auto iter = i->find(ns);
		if(iter == i->end()){
			continue;
		}
		ASSERT(ns == iter->first)
		return &iter->second;
	}
	return nullptr;
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

const std::string* Parser::findAttribute(const std::string& name) {
	auto i = this->attributes.find(name);
	if(i != this->attributes.end()){
		return &i->second;
	}
	return nullptr;
}


const std::string* Parser::findAttributeOfNamespace(XmlNamespace_e ns, const std::string& name){
	if(this->defaultNamespaceStack.back() == ns){
		if(auto a = this->findAttribute(name)){
			return a;
		}
	}
	
	if(auto prefix = this->findFlippedNamespace(ns)){
		if(auto a = this->findAttribute(*prefix + ":" + name)){
			return a;
		}
	}
	return nullptr;
}


void Parser::fillElement(Element& e) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "id")){
		e.id = *a;
	}
}

void Parser::fillGradient(Gradient& g) {
	this->fillElement(g);
	this->fillReferencing(g);
	this->fillStyleable(g);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "spreadMethod")){
		g.spreadMethod = gradientStringToSpreadMethod(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientTransform")){
		g.transformations = Transformable::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientUnits")){
		g.units = parseCoordinateUnits(*a);
	}
}

void Parser::fillRectangle(Rectangle& r, const Rectangle& defaultValues) {
	r = defaultValues;
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x")){
		r.x = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y")){
		r.y = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "width")){
		r.width = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "height")){
		r.height = Length::parse(*a);
	}
}

void Parser::fillReferencing(Referencing& e) {
	auto a = this->findAttributeOfNamespace(XmlNamespace_e::XLINK, "href");
	if(!a){
		a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "href");//in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
	}
	if(a){
		e.iri = *a;
	}
}

void Parser::fillShape(Shape& s) {
	this->fillElement(s);
	this->fillStyleable(s);
	this->fillTransformable(s);
}

void Parser::fillStyleable(Styleable& s) {
	ASSERT(s.styles.size() == 0)

	for(auto& a : this->attributes){
		auto nsn = this->getNamespace(a.first);
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "style") {
					s.styles = Styleable::parse(a.second);
					break;
				}
				{
					StyleProperty_e type = Styleable::stringToProperty(nsn.name);
					if (type != StyleProperty_e::UNKNOWN) {
						s.styles[type] = Styleable::parseStylePropertyValue(type, a.second);
					}
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillTransformable(Transformable& t) {
	ASSERT(t.transformations.size() == 0)
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "transform")){
		t.transformations = Transformable::parse(*a);
	}
}

void Parser::fillViewBoxed(ViewBoxed& v) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "viewBox")){
		v.viewBox = SvgElement::parseViewbox(*a);
	}
}

void Parser::fillTextPositioning(TextPositioning& p) {
	//TODO: parse missing attributes
}

void Parser::fillAspectRatioed(AspectRatioed& e) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "preserveAspectRatio")){
		e.parseAndFillPreserveAspectRatio(*a);
	}
}

void Parser::addElement(std::unique_ptr<Element> e) {
	this->addElement(std::move(e), nullptr);
}

void Parser::addElement(std::unique_ptr<Element> e, Container* c) {
	ASSERT(e)
	ASSERT(this->parentStack.back())
	this->parentStack.back()->children.push_back(std::move(e));
	this->parentStack.push_back(c);
}



void Parser::parseCircleElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "circle")

	auto ret = utki::makeUnique<CircleElement>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "r")){
		ret->r = Length::parse(*a);
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

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseMaskElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "mask")

	auto ret = utki::makeUnique<MaskElement>();

	this->fillElement(*ret);
	this->fillRectangle(*ret);
	this->fillStyleable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskUnits")){
		ret->maskUnits = parseCoordinateUnits(*a);
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskContentUnits")){
		ret->maskContentUnits = parseCoordinateUnits(*a);
	}
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseTextElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "text")

	auto ret = utki::makeUnique<TextElement>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillTransformable(*ret);
	this->fillTextPositioning(*ret);
	
	//TODO: parse missing text element attributes
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
}


void Parser::parseEllipseElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "ellipse")

	auto ret = utki::makeUnique<EllipseElement>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "rx")){
		ret->rx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "ry")){
		ret->ry = Length::parse(*a);
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

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseGradientStopElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "stop")

	auto ret = utki::makeUnique<Gradient::StopElement>();
	
	this->fillStyleable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "offset")){
		std::istringstream s(*a);
		s >> ret->offset;
		if (!s.eof() && s.peek() == '%') {
			ret->offset /= 100;
		}
	}

	this->addElement(std::move(ret));
}

void Parser::parseLineElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "line")

	auto ret = utki::makeUnique<LineElement>();

	this->fillShape(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x1")){
		ret->x1 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y1")){
		ret->y1 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x2")){
		ret->x2 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y2")){
		ret->y2 = Length::parse(*a);
	}


	this->addElement(std::move(ret));
}

void Parser::parseFilterElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "filter")
	
	auto ret = utki::makeUnique<FilterElement>();
	
	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(
			*ret,
			Rectangle(
					Length::make(-10, Length::Unit_e::PERCENT),
					Length::make(-10, Length::Unit_e::PERCENT),
					Length::make(120, Length::Unit_e::PERCENT),
					Length::make(120, Length::Unit_e::PERCENT)
				)
		);
	this->fillReferencing(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "filterUnits")){
		ret->filterUnits = svgdom::parseCoordinateUnits(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "primitiveUnits")){
		ret->primitiveUnits = svgdom::parseCoordinateUnits(*a);
	}
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::fillFilterPrimitive(FilterPrimitive& p) {
	this->fillElement(p);
	this->fillRectangle(p);
	this->fillStyleable(p);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "result")){
		p.result = *a;
	}
}

void Parser::fillInputable(Inputable& p) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in")){
		p.in = *a;
	}
}

void Parser::fillSecondInputable(SecondInputable& p) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in2")){
		p.in2 = *a;
	}
}


void Parser::parseFeGaussianBlurElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "feGaussianBlur")
	
	auto ret = utki::makeUnique<FeGaussianBlurElement>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "stdDeviation")){
		ret->stdDeviation = parseNumberOptionalNumber(*a, {{-1, -1}});
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeColorMatrixElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "feColorMatrix")
	
	auto ret = utki::makeUnique<FeColorMatrixElement>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "type")){
		if(*a == "saturate"){
			ret->type = FeColorMatrixElement::Type_e::SATURATE;
		}else if(*a == "hueRotate"){
			ret->type = FeColorMatrixElement::Type_e::HUE_ROTATE;
		}else if(*a == "luminanceToAlpha"){
			ret->type = FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA;
		}else{
			ret->type = FeColorMatrixElement::Type_e::MATRIX; // default value
		}
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "values")){
		switch(ret->type){
			default:
				ASSERT(false) //should never get here, MATRIX should always be the default value
				break;
			case FeColorMatrixElement::Type_e::MATRIX:
				//20 values expected
				{
					std::istringstream ss(*a);
					
					for(unsigned i = 0; i != 20; ++i){
						ret->values[i] = readInReal(ss);
						if(ss.fail()){
							throw svgdom::Exc("malformed 'values' string of 'feColorMatrix' element");
						}
						skipWhitespacesAndOrComma(ss);
					}
				}
				break;
			case FeColorMatrixElement::Type_e::HUE_ROTATE:
				//fall-through
			case FeColorMatrixElement::Type_e::SATURATE:
				//one value is expected
				{
					std::istringstream ss(*a);
					ret->values[0] = readInReal(ss);
					if(ss.fail()){
						throw svgdom::Exc("malformed 'values' string of 'feColorMatrix' element");
					}
				}
				break;
			case FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA:
				//no values are expected
				break;
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeBlendElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "feBlend")
	
	auto ret = utki::makeUnique<FeBlendElement>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	this->fillSecondInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "mode")){
		if(*a == "normal"){
			ret->mode = FeBlendElement::Mode_e::NORMAL;
		}else if(*a == "multiply"){
			ret->mode = FeBlendElement::Mode_e::MULTIPLY;
		}else if(*a == "screen"){
			ret->mode = FeBlendElement::Mode_e::SCREEN;
		}else if(*a == "darken"){
			ret->mode = FeBlendElement::Mode_e::DARKEN;
		}else if(*a == "lighten"){
			ret->mode = FeBlendElement::Mode_e::LIGHTEN;
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeCompositeElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "feComposite")
	
	auto ret = utki::makeUnique<FeCompositeElement>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	this->fillSecondInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "operator")){
		if(*a == "over"){
			ret->operator_v = FeCompositeElement::Operator_e::OVER;
		}else if(*a == "in"){
			ret->operator_v = FeCompositeElement::Operator_e::IN;
		}else if(*a == "out"){
			ret->operator_v = FeCompositeElement::Operator_e::OUT;
		}else if(*a == "atop"){
			ret->operator_v = FeCompositeElement::Operator_e::ATOP;
		}else if(*a == "xor"){
			ret->operator_v = FeCompositeElement::Operator_e::XOR;
		}else if(*a == "arithmetic"){
			ret->operator_v = FeCompositeElement::Operator_e::ARITHMETIC;
		}
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "k1")){
		ret->k1 = real(std::strtod(a->c_str(), nullptr));
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "k2")){
		ret->k2 = real(std::strtod(a->c_str(), nullptr));
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "k3")){
		ret->k3 = real(std::strtod(a->c_str(), nullptr));
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "k4")){
		ret->k4 = real(std::strtod(a->c_str(), nullptr));
	}
	
	this->addElement(std::move(ret));
}


void Parser::parseLinearGradientElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "linearGradient")

	auto ret = utki::makeUnique<LinearGradientElement>();

	this->fillGradient(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x1")){
		ret->x1 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y1")){
		ret->y1 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x2")){
		ret->x2 = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y2")){
		ret->y2 = Length::parse(*a);
	}

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parsePathElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "path")

	auto ret = utki::makeUnique<PathElement>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "d")){
		ret->path = PathElement::parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolygonElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "polygon")

	auto ret = utki::makeUnique<PolygonElement>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolylineElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "polyline")

	auto ret = utki::makeUnique<PolylineElement>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseRadialGradientElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "radialGradient")

	auto ret = utki::makeUnique<RadialGradientElement>();

	this->fillGradient(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "r")){
		ret->r = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "fx")){
		ret->fx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "fy")){
		ret->fy = Length::parse(*a);
	}

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseRectElement() {
	ASSERT(this->getNamespace(this->element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->element).name == "rect")

	auto ret = utki::makeUnique<RectElement>();

	this->fillShape(*ret);
	this->fillRectangle(*ret, RectElement::rectangleDefaultValues);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "rx")){
		ret->rx = Length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "ry")){
		ret->ry = Length::parse(*a);
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
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
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

	auto c = ret.get();
	this->addElement(std::move(ret), c);
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
	this->popNamespaces();
	this->parentStack.pop_back();
}

void Parser::onAttributeParsed(const utki::Buf<char> name, const utki::Buf<char> value) {
	ASSERT(this->element.length() != 0)
	this->attributes[utki::toString(name)] = utki::toString(value);
}

void Parser::onAttributesEnd(bool isEmptyElement) {
//	TRACE(<< "this->element = " << this->element << std::endl)
	ASSERT(this->parentStack.size() != 0)//there should always be at least root added as parent
//	TRACE(<< "this->parentStack.size() = " << this->parentStack.size() << std::endl)
	this->pushNamespaces();
	if(this->parentStack.back()){
		this->parseNode();
	}else{
		this->parentStack.push_back(nullptr);
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
