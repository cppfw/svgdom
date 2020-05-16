#include "Parser.hxx"
#include "util.hxx"

#include <utki/debug.hpp>
#include <utki/util.hpp>
#include <utki/string.hpp>

#include "malformed_svg_error.hpp"

using namespace svgdom;

namespace{
const std::string DSvgNamespace = "http://www.w3.org/2000/svg";
const std::string DXlinkNamespace = "http://www.w3.org/1999/xlink";
}

namespace{
gradient::spread_method gradientStringToSpreadMethod(const std::string& str) {
	if(str == "pad"){
		return gradient::spread_method::pad;
	}else if(str == "reflect"){
		return gradient::spread_method::reflect;
	}else if(str == "repeat"){
		return gradient::spread_method::repeat;
	}
	return gradient::spread_method::default_;
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
	auto nsn = this->getNamespace(this->cur_element);
//	TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch(nsn.ns){
		case XmlNamespace_e::SVG:
			//TODO: optimize using map?
			if(nsn.name == svg_element::tag){
				this->parseSvgElement();
			}else if(nsn.name == symbol_element::tag) {
				this->parseSymbolElement();
			}else if(nsn.name == g_element::tag){
				this->parseGElement();
			}else if(nsn.name == defs_element::tag){
				this->parseDefsElement();
			}else if (nsn.name == use_element::tag) {
				this->parseUseElement();
			}else if(nsn.name == path_element::tag){
				this->parsePathElement();
			}else if(nsn.name == linear_gradient_element::tag){
				this->parseLinearGradientElement();
			}else if(nsn.name == radial_gradient_element::tag){
				this->parseRadialGradientElement();
			}else if(nsn.name == gradient::stop_element::tag){
				this->parseGradientStopElement();
			}else if(nsn.name == rect_element::tag){
				this->parseRectElement();
			}else if(nsn.name == circle_element::tag){
				this->parseCircleElement();
			}else if(nsn.name == ellipse_element::tag){
				this->parseEllipseElement();
			}else if(nsn.name == line_element::tag){
				this->parseLineElement();
			}else if(nsn.name == polyline_element::tag){
				this->parsePolylineElement();
			}else if(nsn.name == polygon_element::tag){
				this->parsePolygonElement();
			}else if(nsn.name == filter_element::tag){
				this->parseFilterElement();
			}else if(nsn.name == fe_gaussian_blur_element::tag){
				this->parseFeGaussianBlurElement();
			}else if(nsn.name == fe_color_matrix_element::tag){
				this->parseFeColorMatrixElement();
			}else if(nsn.name == fe_blend_element::tag){
				this->parseFeBlendElement();
			}else if(nsn.name == fe_composite_element::tag){
				this->parseFeCompositeElement();
			}else if(nsn.name == image_element::tag){
				this->parseImageElement();
			}else if(nsn.name == mask_element::tag){
				this->parseMaskElement();
			}else if(nsn.name == text_element::tag){
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


void Parser::fillElement(element& e) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "id")){
		e.id = *a;
	}
}

void Parser::fillGradient(gradient& g) {
	this->fillElement(g);
	this->fillReferencing(g);
	this->fillStyleable(g);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "spreadMethod")){
		g.spreadMethod = gradientStringToSpreadMethod(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientTransform")){
		g.transformations = transformable::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientUnits")){
		g.units = parseCoordinateUnits(*a);
	}
}

void Parser::fillRectangle(rectangle& r, const rectangle& defaultValues) {
	r = defaultValues;
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x")){
		r.x = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y")){
		r.y = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "width")){
		r.width = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "height")){
		r.height = length::parse(*a);
	}
}

void Parser::fillReferencing(referencing& e) {
	auto a = this->findAttributeOfNamespace(XmlNamespace_e::XLINK, "href");
	if(!a){
		a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "href");//in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
	}
	if(a){
		e.iri = *a;
	}
}

void Parser::fillShape(shape& s) {
	this->fillElement(s);
	this->fillStyleable(s);
	this->fillTransformable(s);
}

void Parser::fillStyleable(styleable& s) {
	ASSERT(s.styles.size() == 0)

	for(auto& a : this->attributes){
		auto nsn = this->getNamespace(a.first);
		switch (nsn.ns) {
			case XmlNamespace_e::SVG:
				if (nsn.name == "style") {
					s.styles = styleable::parse(a.second);
					break;
				}
				{
					style_property type = styleable::stringToProperty(nsn.name);
					if (type != style_property::UNKNOWN) {
						s.styles[type] = styleable::parseStylePropertyValue(type, a.second);
					}
				}
				break;
			default:
				break;
		}
	}
}

void Parser::fillTransformable(transformable& t) {
	ASSERT(t.transformations.size() == 0)
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "transform")){
		t.transformations = transformable::parse(*a);
	}
}

void Parser::fillViewBoxed(view_boxed& v) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "viewBox")){
		v.viewBox = svg_element::parseViewbox(*a);
	}
}

void Parser::fillTextPositioning(text_positioning& p) {
	//TODO: parse missing attributes
}

void Parser::fillAspectRatioed(aspect_ratioed& e) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "preserveAspectRatio")){
		e.preserve_aspect_ratio.parse(*a);
	}
}

void Parser::addElement(std::unique_ptr<element> e) {
	this->addElement(std::move(e), nullptr);
}

void Parser::addElement(std::unique_ptr<element> e, container* c) {
	ASSERT(e)
	ASSERT(this->parentStack.back())
	this->parentStack.back()->children.push_back(std::move(e));
	this->parentStack.push_back(c);
}



void Parser::parseCircleElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == circle_element::tag)

	auto ret = std::make_unique<circle_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "r")){
		ret->r = length::parse(*a);
	}

	this->addElement(std::move(ret));
}

void Parser::parseDefsElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == defs_element::tag)

	auto ret = std::make_unique<defs_element>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseMaskElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == mask_element::tag)

	auto ret = std::make_unique<mask_element>();

	this->fillElement(*ret);
	this->fillRectangle(*ret);
	this->fillStyleable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskUnits")){
		ret->mask_units = parseCoordinateUnits(*a);
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskContentUnits")){
		ret->mask_content_units = parseCoordinateUnits(*a);
	}
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseTextElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == text_element::tag)

	auto ret = std::make_unique<text_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillTransformable(*ret);
	this->fillTextPositioning(*ret);
	
	//TODO: parse missing text element attributes
	
	auto c = ret.get();
	this->addElement(std::move(ret), c);
}


void Parser::parseEllipseElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == ellipse_element::tag)

	auto ret = std::make_unique<ellipse_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "rx")){
		ret->rx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "ry")){
		ret->ry = length::parse(*a);
	}

	this->addElement(std::move(ret));
}

void Parser::parseGElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == g_element::tag)

	auto ret = std::make_unique<g_element>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseGradientStopElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == gradient::stop_element::tag)

	auto ret = std::make_unique<gradient::stop_element>();
	
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
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == line_element::tag)

	auto ret = std::make_unique<line_element>();

	this->fillShape(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x1")){
		ret->x1 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y1")){
		ret->y1 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x2")){
		ret->x2 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y2")){
		ret->y2 = length::parse(*a);
	}


	this->addElement(std::move(ret));
}

void Parser::parseFilterElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == filter_element::tag)
	
	auto ret = std::make_unique<filter_element>();
	
	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(
			*ret,
			rectangle(
					length(-10, length_unit::PERCENT),
					length(-10, length_unit::PERCENT),
					length(120, length_unit::PERCENT),
					length(120, length_unit::PERCENT)
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

void Parser::fillFilterPrimitive(filter_primitive& p) {
	this->fillElement(p);
	this->fillRectangle(p);
	this->fillStyleable(p);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "result")){
		p.result = *a;
	}
}

void Parser::fillInputable(inputable& p) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in")){
		p.in = *a;
	}
}

void Parser::fillSecondInputable(second_inputable& p) {
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in2")){
		p.in2 = *a;
	}
}


void Parser::parseFeGaussianBlurElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_gaussian_blur_element::tag)
	
	auto ret = std::make_unique<fe_gaussian_blur_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "stdDeviation")){
		ret->stdDeviation = parseNumberOptionalNumber(*a, {{-1, -1}});
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeColorMatrixElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_color_matrix_element::tag)
	
	auto ret = std::make_unique<fe_color_matrix_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "type")){
		if(*a == "saturate"){
			ret->type_ = fe_color_matrix_element::Type_e::SATURATE;
		}else if(*a == "hueRotate"){
			ret->type_ = fe_color_matrix_element::Type_e::HUE_ROTATE;
		}else if(*a == "luminanceToAlpha"){
			ret->type_ = fe_color_matrix_element::Type_e::LUMINANCE_TO_ALPHA;
		}else{
			ret->type_ = fe_color_matrix_element::Type_e::MATRIX; // default value
		}
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "values")){
		switch(ret->type_){
			default:
				ASSERT(false) // should never get here, MATRIX should always be the default value
				break;
			case fe_color_matrix_element::Type_e::MATRIX:
				// 20 values expected
				{
					std::istringstream ss(*a);
					
					for(unsigned i = 0; i != 20; ++i){
						ret->values[i] = readInReal(ss);
						if(ss.fail()){
							throw malformed_svg_error("malformed 'values' string of 'feColorMatrix' element");
						}
						skipWhitespacesAndOrComma(ss);
					}
				}
				break;
			case fe_color_matrix_element::Type_e::HUE_ROTATE:
				// fall-through
			case fe_color_matrix_element::Type_e::SATURATE:
				// one value is expected
				{
					std::istringstream ss(*a);
					ret->values[0] = readInReal(ss);
					if(ss.fail()){
						throw malformed_svg_error("malformed 'values' string of 'feColorMatrix' element");
					}
				}
				break;
			case fe_color_matrix_element::Type_e::LUMINANCE_TO_ALPHA:
				// no values are expected
				break;
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeBlendElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_blend_element::tag)
	
	auto ret = std::make_unique<fe_blend_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	this->fillSecondInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "mode")){
		if(*a == "normal"){
			ret->mode_ = FeBlendElement::Mode_e::NORMAL;
		}else if(*a == "multiply"){
			ret->mode_ = FeBlendElement::Mode_e::MULTIPLY;
		}else if(*a == "screen"){
			ret->mode_ = FeBlendElement::Mode_e::SCREEN;
		}else if(*a == "darken"){
			ret->mode_ = FeBlendElement::Mode_e::DARKEN;
		}else if(*a == "lighten"){
			ret->mode_ = FeBlendElement::Mode_e::LIGHTEN;
		}
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseFeCompositeElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_composite_element::tag)
	
	auto ret = std::make_unique<fe_composite_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	this->fillSecondInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "operator")){
		if(*a == "over"){
			ret->operator__ = fe_composite_element::operator_::over;
		}else if(*a == "in"){
			ret->operator__ = fe_composite_element::operator_::in;
		}else if(*a == "out"){
			ret->operator__ = fe_composite_element::operator_::out;
		}else if(*a == "atop"){
			ret->operator__ = fe_composite_element::operator_::atop;
		}else if(*a == "xor"){
			ret->operator__ = fe_composite_element::operator_::xor_;
		}else if(*a == "arithmetic"){
			ret->operator__ = fe_composite_element::operator_::arithmetic;
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
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == linear_gradient_element::tag)

	auto ret = std::make_unique<linear_gradient_element>();

	this->fillGradient(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x1")){
		ret->x1 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y1")){
		ret->y1 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "x2")){
		ret->x2 = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "y2")){
		ret->y2 = length::parse(*a);
	}

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parsePathElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == path_element::tag)

	auto ret = std::make_unique<path_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "d")){
		ret->path = path_element::parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolygonElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == polygon_element::tag)

	auto ret = std::make_unique<polygon_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parsePolylineElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == polyline_element::tag)

	auto ret = std::make_unique<polyline_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void Parser::parseRadialGradientElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == radial_gradient_element::tag)

	auto ret = std::make_unique<radial_gradient_element>();

	this->fillGradient(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cx")){
		ret->cx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "cy")){
		ret->cy = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "r")){
		ret->r = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "fx")){
		ret->fx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "fy")){
		ret->fy = length::parse(*a);
	}

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseRectElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == rect_element::tag)

	auto ret = std::make_unique<rect_element>();

	this->fillShape(*ret);
	this->fillRectangle(*ret, rect_element::rectangle_default_values());

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "rx")){
		ret->rx = length::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "ry")){
		ret->ry = length::parse(*a);
	}

	this->addElement(std::move(ret));
}

void Parser::parseSvgElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == svg_element::tag)

	auto ret = std::make_unique<svg_element>();

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
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == image_element::tag)

	auto ret = std::make_unique<image_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillTransformable(*ret);
	this->fillRectangle(*ret);
	this->fillReferencing(*ret);
	this->fillAspectRatioed(*ret);

	this->addElement(std::move(ret));
}


void Parser::parseSymbolElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == symbol_element::tag)

	//		TRACE(<< "parseSymbolElement():" << std::endl)

	auto ret = std::make_unique<symbol_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillViewBoxed(*ret);
	this->fillAspectRatioed(*ret);

	auto c = ret.get();
	this->addElement(std::move(ret), c);
}

void Parser::parseUseElement() {
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == use_element::tag)

	auto ret = std::make_unique<use_element>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);
	this->fillReferencing(*ret);
	this->fillRectangle(*ret);

	this->addElement(std::move(ret));
}

void Parser::on_element_start(utki::span<const char> name) {
	this->cur_element = utki::make_string(name);
}

void Parser::on_element_end(utki::span<const char> name) {
	this->popNamespaces();
	this->parentStack.pop_back();
}

void Parser::on_attribute_parsed(utki::span<const char> name, utki::span<const char> value) {
	ASSERT(this->cur_element.length() != 0)
	this->attributes[utki::make_string(name)] = utki::make_string(value);
}

void Parser::on_attributes_end(bool is_empty_element) {
//	TRACE(<< "this->cur_element = " << this->cur_element << std::endl)
	ASSERT(this->parentStack.size() != 0)//there should always be at least root added as parent
//	TRACE(<< "this->parentStack.size() = " << this->parentStack.size() << std::endl)
	this->pushNamespaces();
	if(this->parentStack.back()){
		this->parseNode();
	}else{
		this->parentStack.push_back(nullptr);
	}
	this->attributes.clear();
	this->cur_element.clear();
}

void Parser::on_content_parsed(utki::span<const char> str) {
	// do nothing for now
}

std::unique_ptr<svg_element> Parser::getDom() {
	if(!this->svg){
		return nullptr;
	}
	ASSERT(this->root.children.size() != 0)
	
	this->root.children.front().release();
	this->root.children.clear();
	return std::unique_ptr<svg_element>(this->svg);
}
