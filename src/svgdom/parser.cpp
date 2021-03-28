#include "parser.hxx"
#include "util.hxx"
#include "malformed_svg_error.hpp"
#include "util/casters.hpp"

#include <utki/debug.hpp>
#include <utki/util.hpp>
#include <utki/string.hpp>

#include <papki/span_file.hpp>

using namespace svgdom;

namespace{
const std::string DSvgNamespace = "http://www.w3.org/2000/svg";
const std::string DXlinkNamespace = "http://www.w3.org/1999/xlink";
}

namespace{
gradient::spread_method gradientStringToSpreadMethod(const std::string& str){
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

void parser::pushNamespaces(){
	// parse default namespace
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
		
		this->flippedNamespacesStack.push_back(utki::flip_map(this->namespacesStack.back()));
	}
}

void parser::popNamespaces(){
	ASSERT(this->namespacesStack.size() != 0)
	this->namespacesStack.pop_back();
	ASSERT(this->defaultNamespaceStack.size() != 0)
	this->defaultNamespaceStack.pop_back();
	ASSERT(this->flippedNamespacesStack.size() != 0)
	this->flippedNamespacesStack.pop_back();
}

void parser::parse_element(){
	auto nsn = this->getNamespace(this->cur_element);
	// TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch(nsn.ns){
		case XmlNamespace_e::SVG:
			if(nsn.name == svg_element::tag){
				this->parseSvgElement();
			}else if(nsn.name == symbol_element::tag){
				this->parseSymbolElement();
			}else if(nsn.name == g_element::tag){
				this->parseGElement();
			}else if(nsn.name == defs_element::tag){
				this->parseDefsElement();
			}else if(nsn.name == use_element::tag){
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
			}else if(nsn.name == style_element::tag){
				this->parse_style_element();
			}else{
				// unknown element, ignore
				break;
			}
			return;
		default:
			// unknown namespace, ignore
			break;
	}
	this->element_stack.push_back(nullptr);
}

parser::XmlNamespace_e parser::find_namespace(const std::string& ns){
	for(auto i = this->namespacesStack.rbegin(), e = this->namespacesStack.rend(); i != e; ++i){
		auto iter = i->find(ns);
		if(iter == i->end()){
			continue;
		}
		ASSERT(ns == iter->first)
		return iter->second;
	}
	return XmlNamespace_e::UNKNOWN;
}

const std::string* parser::findFlippedNamespace(XmlNamespace_e ns){
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

parser::NamespaceNamePair parser::getNamespace(const std::string& xmlName){
	NamespaceNamePair ret;

	auto colonIndex = xmlName.find_first_of(':');
	if(colonIndex == std::string::npos){
		ret.ns = this->defaultNamespaceStack.back();
		ret.name = xmlName;
		return ret;
	}

	ASSERT(xmlName.length() >= colonIndex + 1)

	ret.ns = this->find_namespace(xmlName.substr(0, colonIndex));
	ret.name = xmlName.substr(colonIndex + 1, xmlName.length() - 1 - colonIndex);

	return ret;
}

const std::string* parser::findAttribute(const std::string& name){
	auto i = this->attributes.find(name);
	if(i != this->attributes.end()){
		return &i->second;
	}
	return nullptr;
}

const std::string* parser::findAttributeOfNamespace(XmlNamespace_e ns, const std::string& name){
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

void parser::fillElement(element& e){
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "id")){
		e.id = *a;
	}
}

void parser::fillGradient(gradient& g){
	this->fillElement(g);
	this->fillReferencing(g);
	this->fillStyleable(g);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "spreadMethod")){
		g.spread_method_ = gradientStringToSpreadMethod(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientTransform")){
		g.transformations = transformable::parse(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "gradientUnits")){
		g.units = parse_coordinate_units(*a);
	}
}

void parser::fillRectangle(rectangle& r, const rectangle& defaultValues){
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

void parser::fillReferencing(referencing& e){
	auto a = this->findAttributeOfNamespace(XmlNamespace_e::XLINK, "href");
	if(!a){
		a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "href");//in some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
	}
	if(a){
		e.iri = *a;
	}
}

void parser::fillShape(shape& s){
	this->fillElement(s);
	this->fillStyleable(s);
	this->fillTransformable(s);
}

void parser::fillStyleable(styleable& s){
	ASSERT(s.styles.size() == 0)

	for(auto& a : this->attributes){
		auto nsn = this->getNamespace(a.first);
		switch (nsn.ns){
			case XmlNamespace_e::SVG:
				if(nsn.name == "style"){
					s.styles = styleable::parse(a.second);
					break;
				}else if(nsn.name == "class"){
					s.classes = utki::split(a.second);
					break;
				}

				// parse style attributes
				{
					style_property type = styleable::string_to_property(nsn.name);
					if(type != style_property::unknown){
						s.presentation_attributes[type] = styleable::parse_style_property_value(type, a.second);
					}
				}
				break;
			default:
				break;
		}
	}
}

void parser::fillTransformable(transformable& t){
	ASSERT(t.transformations.size() == 0)
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "transform")){
		t.transformations = transformable::parse(*a);
	}
}

void parser::fillViewBoxed(view_boxed& v){
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "viewBox")){
		v.view_box = svg_element::parse_view_box(*a);
	}
}

void parser::fillTextPositioning(text_positioning& p){
	// TODO: parse missing attributes
}

void parser::fill_style(style_element& e){
	// TODO: parse missing attributes
}

void parser::fillAspectRatioed(aspect_ratioed& e){
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "preserveAspectRatio")){
		e.preserve_aspect_ratio.parse(*a);
	}
}

void parser::addElement(std::unique_ptr<element> e){
	ASSERT(e)
	
	auto elem = e.get();

	if(this->element_stack.empty()){
		if(this->svg){
			throw malformed_svg_error("more than one root element found in the SVG document");
		}

		element_caster<svg_element> c;
		e->accept(c);
		if(!c.pointer){
			throw malformed_svg_error("first element of the SVG document is not an 'svg' element");
		}
		
		e.release();
		this->svg = decltype(this->svg)(c.pointer);
	}else{
		container_caster c;
		auto parent = this->element_stack.back();
		if(parent){
			parent->accept(c);
		}
		if(c.pointer){
			c.pointer->children.push_back(std::move(e));
		}else{
			elem = nullptr;
		}
	}
	this->element_stack.push_back(elem);
}

void parser::parseCircleElement(){
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

void parser::parseDefsElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == defs_element::tag)

	auto ret = std::make_unique<defs_element>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	this->addElement(std::move(ret));
}

void parser::parseMaskElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == mask_element::tag)

	auto ret = std::make_unique<mask_element>();

	this->fillElement(*ret);
	this->fillRectangle(*ret);
	this->fillStyleable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskUnits")){
		ret->mask_units = parse_coordinate_units(*a);
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "maskContentUnits")){
		ret->mask_content_units = parse_coordinate_units(*a);
	}
	
	this->addElement(std::move(ret));
}

void parser::parseTextElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == text_element::tag)

	auto ret = std::make_unique<text_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillTransformable(*ret);
	this->fillTextPositioning(*ret);
	
	//TODO: parse missing text element attributes
	
	this->addElement(std::move(ret));
}

void parser::parse_style_element(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == style_element::tag)

	auto ret = std::make_unique<style_element>();

	this->fillElement(*ret);
	this->fill_style(*ret);
	
	// TODO: parse missing style element attributes
	
	this->addElement(std::move(ret));
}

void parser::parseEllipseElement(){
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

void parser::parseGElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == g_element::tag)

	auto ret = std::make_unique<g_element>();

	this->fillElement(*ret);
	this->fillTransformable(*ret);
	this->fillStyleable(*ret);

	this->addElement(std::move(ret));
}

void parser::parseGradientStopElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == gradient::stop_element::tag)

	auto ret = std::make_unique<gradient::stop_element>();
	
	this->fillStyleable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "offset")){
		std::istringstream s(*a);
		s >> ret->offset;
		if(!s.eof() && s.peek() == '%'){
			ret->offset /= 100;
		}
	}

	this->addElement(std::move(ret));
}

void parser::parseLineElement(){
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

void parser::parseFilterElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == filter_element::tag)
	
	auto ret = std::make_unique<filter_element>();
	
	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(
			*ret,
			rectangle(
					length(-10, length_unit::percent),
					length(-10, length_unit::percent),
					length(120, length_unit::percent),
					length(120, length_unit::percent)
				)
		);
	this->fillReferencing(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "filterUnits")){
		ret->filter_units = svgdom::parse_coordinate_units(*a);
	}
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "primitiveUnits")){
		ret->primitive_units = svgdom::parse_coordinate_units(*a);
	}
	
	this->addElement(std::move(ret));
}

void parser::fillFilterPrimitive(filter_primitive& p){
	this->fillElement(p);
	this->fillRectangle(p);
	this->fillStyleable(p);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "result")){
		p.result = *a;
	}
}

void parser::fillInputable(inputable& p){
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in")){
		p.in = *a;
	}
}

void parser::fillSecondInputable(second_inputable& p){
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "in2")){
		p.in2 = *a;
	}
}

void parser::parseFeGaussianBlurElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_gaussian_blur_element::tag)
	
	auto ret = std::make_unique<fe_gaussian_blur_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "stdDeviation")){
		ret->std_deviation = parse_number_and_optional_number(*a, {-1, -1});
	}
	
	this->addElement(std::move(ret));
}

void parser::parseFeColorMatrixElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_color_matrix_element::tag)
	
	auto ret = std::make_unique<fe_color_matrix_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "type")){
		if(*a == "saturate"){
			ret->type_ = fe_color_matrix_element::type::saturate;
		}else if(*a == "hueRotate"){
			ret->type_ = fe_color_matrix_element::type::hue_rotate;
		}else if(*a == "luminanceToAlpha"){
			ret->type_ = fe_color_matrix_element::type::luminance_to_alpha;
		}else{
			ret->type_ = fe_color_matrix_element::type::matrix; // default value
		}
	}
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "values")){
		switch(ret->type_){
			default:
				ASSERT(false) // should never get here, MATRIX should always be the default value
				break;
			case fe_color_matrix_element::type::matrix:
				// 20 values expected
				{
					std::istringstream ss(*a);
					
					for(unsigned i = 0; i != 20; ++i){
						ret->values[i] = read_in_real(ss);
						if(ss.fail()){
							throw malformed_svg_error("malformed 'values' string of 'feColorMatrix' element");
						}
						skip_whitespaces_and_comma(ss);
					}
				}
				break;
			case fe_color_matrix_element::type::hue_rotate:
				// fall-through
			case fe_color_matrix_element::type::saturate:
				// one value is expected
				{
					std::istringstream ss(*a);
					ret->values[0] = read_in_real(ss);
					if(ss.fail()){
						throw malformed_svg_error("malformed 'values' string of 'feColorMatrix' element");
					}
				}
				break;
			case fe_color_matrix_element::type::luminance_to_alpha:
				// no values are expected
				break;
		}
	}
	
	this->addElement(std::move(ret));
}

void parser::parseFeBlendElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == fe_blend_element::tag)
	
	auto ret = std::make_unique<fe_blend_element>();
	
	this->fillFilterPrimitive(*ret);
	this->fillInputable(*ret);
	this->fillSecondInputable(*ret);
	
	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "mode")){
		if(*a == "normal"){
			ret->mode_ = fe_blend_element::mode::normal;
		}else if(*a == "multiply"){
			ret->mode_ = fe_blend_element::mode::multiply;
		}else if(*a == "screen"){
			ret->mode_ = fe_blend_element::mode::screen;
		}else if(*a == "darken"){
			ret->mode_ = fe_blend_element::mode::darken;
		}else if(*a == "lighten"){
			ret->mode_ = fe_blend_element::mode::lighten;
		}
	}
	
	this->addElement(std::move(ret));
}

void parser::parseFeCompositeElement(){
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

void parser::parseLinearGradientElement(){
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

	this->addElement(std::move(ret));
}

void parser::parsePathElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == path_element::tag)

	auto ret = std::make_unique<path_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "d")){
		ret->path = path_element::parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void parser::parsePolygonElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == polygon_element::tag)

	auto ret = std::make_unique<polygon_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void parser::parsePolylineElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == polyline_element::tag)

	auto ret = std::make_unique<polyline_element>();

	this->fillShape(*ret);

	if(auto a = this->findAttributeOfNamespace(XmlNamespace_e::SVG, "points")){
		ret->points = ret->parse(*a);
	}
	
	this->addElement(std::move(ret));
}

void parser::parseRadialGradientElement(){
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

	this->addElement(std::move(ret));
}

void parser::parseRectElement(){
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

void parser::parseSvgElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == svg_element::tag)

	auto ret = std::make_unique<svg_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillRectangle(*ret);
	this->fillViewBoxed(*ret);
	this->fillAspectRatioed(*ret);
	
	this->addElement(std::move(ret));
}

void parser::parseImageElement(){
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

void parser::parseSymbolElement(){
	ASSERT(this->getNamespace(this->cur_element).ns == XmlNamespace_e::SVG)
	ASSERT(this->getNamespace(this->cur_element).name == symbol_element::tag)

	//		TRACE(<< "parseSymbolElement():" << std::endl)

	auto ret = std::make_unique<symbol_element>();

	this->fillElement(*ret);
	this->fillStyleable(*ret);
	this->fillViewBoxed(*ret);
	this->fillAspectRatioed(*ret);

	this->addElement(std::move(ret));
}

void parser::parseUseElement(){
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

void parser::on_element_start(utki::span<const char> name){
	this->cur_element = utki::make_string(name);
}

void parser::on_element_end(utki::span<const char> name){
	this->popNamespaces();
	this->element_stack.pop_back();
}

void parser::on_attribute_parsed(utki::span<const char> name, utki::span<const char> value){
	ASSERT(this->cur_element.length() != 0)
	this->attributes[utki::make_string(name)] = utki::make_string(value);
}

void parser::on_attributes_end(bool is_empty_element){
//	TRACE(<< "this->cur_element = " << this->cur_element << std::endl)
//	TRACE(<< "this->element_stack.size() = " << this->element_stack.size() << std::endl)
	this->pushNamespaces();

	this->parse_element();

	this->attributes.clear();
	this->cur_element.clear();
}

namespace{
class parse_content_visitor : public visitor{
	const utki::span<const char> content;
public:
	parse_content_visitor(utki::span<const char> content) :
			content(content)
	{}

	void default_visit(element&, container&)override{
		// do nothing
	}

	void visit(style_element& e)override{
		e.css.append(cssdom::read(
				papki::span_file(this->content),
				[](const std::string& name) -> uint32_t{
					return uint32_t(styleable::string_to_property(name));
				},
				[](uint32_t id, std::string&& v) -> std::unique_ptr<cssdom::property_value_base>{
					auto sp = style_property(id);
					if(sp == style_property::unknown){
						return nullptr;
					}
					auto ret = std::make_unique<style_element::css_style_value>();
					ret->value = styleable::parse_style_property_value(sp, v);
					return ret;
				}
			));
	}
};
}

void parser::on_content_parsed(utki::span<const char> str){
	if(this->element_stack.empty() || !this->element_stack.back()){
		return;
	}

	parse_content_visitor v(str);
	this->element_stack.back()->accept(v);
}

std::unique_ptr<svg_element> parser::get_dom(){
	return std::move(this->svg);
}
