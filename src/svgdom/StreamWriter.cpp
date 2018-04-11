#include "StreamWriter.hpp"

#include <utki/util.hpp>

#include "util.hxx"
#include "Exc.hpp"

using namespace svgdom;

void StreamWriter::setName(const std::string& name) {
	this->name = name;
}

void StreamWriter::addAttribute(const std::string& name, const std::string& value) {
	this->attributes.push_back(std::make_pair(name, value));
}

void StreamWriter::addAttribute(const std::string& name, const Length& value){
	std::stringstream ss;
	ss << value;
	this->addAttribute(name, ss.str());
}

void StreamWriter::addAttribute(const std::string& name, real value){
	std::stringstream ss;
	ss << value;
	this->addAttribute(name, ss.str());
}

void StreamWriter::write(const Container* children) {
	auto ind = indentStr();

	auto tag = std::move(this->name);
	
	this->s << ind << "<" << tag;
	
	for(auto& a : this->attributes){
		this->s << " " << a.first << "=\"" << a.second << "\"";
	}

	this->attributes.clear();
	this->name.clear();
	
	if (!children || children->children.size() == 0) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		this->childrenToStream(*children);
		this->s << ind << "</" << tag << ">";
	}
	this->s << std::endl;
}

std::string StreamWriter::indentStr() {
	std::string ind;

	std::stringstream ss;
	for(unsigned i = 0; i != this->indent; ++i){
		ss << "\t";
	}
	return ss.str();
}

void StreamWriter::childrenToStream(const Container& e) {
	++this->indent;
	utki::ScopeExit scopeExit([this]() {
		--this->indent;
	});
	for (auto& c : e.children) {
		c->accept(*this);
	}
}

void StreamWriter::addElementAttributes(const Element& e) {
	if(e.id.length() != 0){
		this->addAttribute("id", e.id);
	}
}

void StreamWriter::addTransformableAttributes(const Transformable& e) {
	if(e.transformations.size() != 0){
		this->addAttribute("transform", e.transformationsToString());
	}
}

void StreamWriter::addStyleableAttributes(const Styleable& e) {
	if(e.styles.size() != 0){
		this->addAttribute("style", e.stylesToString());
	}
}

void StreamWriter::addViewBoxedAttributes(const ViewBoxed& e) {
	if (e.isViewBoxSpecified()) {
		this->addAttribute("viewBox", e.viewBoxToString());
	}
}

void StreamWriter::addAspectRatioedAttributes(const AspectRatioed& e) {
	if (e.preserveAspectRatio.preserve != AspectRatioed::PreserveAspectRatio_e::NONE || e.preserveAspectRatio.defer || e.preserveAspectRatio.slice) {
		this->addAttribute("preserveAspectRatio", e.preserveAspectRatioToString());
	}
}

void StreamWriter::addRectangleAttributes(const Rectangle& e, const Rectangle& defaultValues) {
	if(e.isXSpecified() && e.x != defaultValues.x){
		this->addAttribute("x", e.x);
	}
	
	if(e.isYSpecified() && e.y != defaultValues.y){
		this->addAttribute("y", e.y);
	}
	
	if(e.isWidthSpecified() && e.width != defaultValues.width){
		this->addAttribute("width", e.width);
	}
	
	if(e.isHeightSpecified() && e.height != defaultValues.height){
		this->addAttribute("height", e.height);
	}
}

void StreamWriter::addReferencingAttributes(const Referencing& e) {
	if(e.iri.length() != 0){
		this->addAttribute("xlink:href", e.iri);
	}
}

void StreamWriter::addGradientAttributes(const Gradient& e) {
	this->addElementAttributes(e);
	this->addReferencingAttributes(e);
	this->addStyleableAttributes(e);
	
	if(e.spreadMethod != Gradient::SpreadMethod_e::DEFAULT){
		this->addAttribute("spreadMethod", e.spreadMethodToString());
	}
	
	if(e.units != CoordinateUnits_e::UNKNOWN){
		this->addAttribute("gradientUnits", coordinateUnitsToString(e.units));
	}
	
	if(e.transformations.size() != 0){
		this->addAttribute("gradientTransform", e.transformationsToString());
	}
}

void StreamWriter::addFilterPrimitiveAttributes(const FilterPrimitive& e) {
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->addRectangleAttributes(e);
	
	if(e.result.length() != 0){
		this->addAttribute("result", e.result);
	}
}

void StreamWriter::addInputableAttributes(const Inputable& e) {
	if(e.in.length() != 0){
		this->addAttribute("in", e.in);
	}
}

void StreamWriter::addSecondInputableAttributes(const SecondInputable& e) {
	if(e.in2.length() != 0){
		this->addAttribute("in2", e.in2);
	}
}

void StreamWriter::addTextPositioningAttributes(const TextPositioning& e) {
	//TODO: add missing attributes
}


void StreamWriter::addShapeAttributes(const Shape& e) {
	this->addElementAttributes(e);
	this->addTransformableAttributes(e);
	this->addStyleableAttributes(e);
}


void StreamWriter::visit(const GElement& e) {
	this->setName("g");
	this->addElementAttributes(e);
	this->addTransformableAttributes(e);
	this->addStyleableAttributes(e);
	this->write(&e);
}

void StreamWriter::visit(const SvgElement& e) {
	this->setName("svg");
	
	if(this->indent == 0){//if outermost "svg" element
		this->addAttribute("xmlns", "http://www.w3.org/2000/svg");
		this->addAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
		this->addAttribute("version", "1.1");
	}
	
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->addRectangleAttributes(e);
	this->addViewBoxedAttributes(e);
	this->addAspectRatioedAttributes(e);
	this->write(&e);
}

void StreamWriter::visit(const ImageElement& e){
	this->setName("image");
	
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->addTransformableAttributes(e);
	this->addRectangleAttributes(
			e,
			Rectangle(
					Length::make(0, Length::Unit_e::NUMBER),
					Length::make(0, Length::Unit_e::NUMBER),
					Length::make(0, Length::Unit_e::NUMBER),
					Length::make(0, Length::Unit_e::NUMBER)
				)
		);
	this->addReferencingAttributes(e);
	this->addAspectRatioedAttributes(e);
	this->write();
}

void StreamWriter::visit(const LineElement& e) {
	this->setName("line");
	this->addShapeAttributes(e);
	
	if(e.x1.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("x1", e.x1);
	}
	
	if(e.y1.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("y1", e.y1);
	}
	
	if(e.x2.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("x2", e.x2);
	}
	
	if(e.y2.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("y2", e.y2);
	}
	
	this->write();
}

void StreamWriter::visit(const RectElement& e) {
	this->setName("rect");
	this->addShapeAttributes(e);
	this->addRectangleAttributes(e, RectElement::rectangleDefaultValues);
	
	if(e.rx.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("rx", e.rx);
	}
	
	if(e.ry.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("ry", e.ry);
	}
	
	this->write();
}

void StreamWriter::visit(const EllipseElement& e) {
	this->setName("ellipse");
	this->addShapeAttributes(e);
	
	if(e.cx.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("cx", e.cx);
	}
	
	if(e.cy.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("cy", e.cy);
	}
	
	if(e.rx.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("rx", e.rx);
	}
	
	if(e.ry.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("ry", e.ry);
	}
	
	this->write();
}

void StreamWriter::visit(const PolygonElement& e) {
	this->setName("polygon");
	this->addShapeAttributes(e);
	if(e.points.size() != 0){
		this->addAttribute("points", e.pointsToString());
	}
	this->write();
}

void StreamWriter::visit(const PolylineElement& e) {
	this->setName("polyline");
	this->addShapeAttributes(e);
	if(e.points.size() != 0){
		this->addAttribute("points", e.pointsToString());
	}
	this->write();
}

void StreamWriter::visit(const CircleElement& e) {
	this->setName("circle");
	this->addShapeAttributes(e);
	
	if(e.cx.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("cx", e.cx);
	}
	
	if(e.cy.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("cy", e.cy);
	}
	
	if(e.r.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("r", e.r);
	}
	this->write();
}

void StreamWriter::visit(const PathElement& e){
	this->setName("path");
	this->addShapeAttributes(e);
	if(e.path.size() != 0){
		this->addAttribute("d", e.pathToString());
	}
	this->write();
}

void StreamWriter::visit(const UseElement& e) {
	this->setName("use");
	this->addElementAttributes(e);
	this->addTransformableAttributes(e);
	this->addStyleableAttributes(e);
	this->addRectangleAttributes(e);
	this->addReferencingAttributes(e);
	this->write();
}

void StreamWriter::visit(const Gradient::StopElement& e) {
	this->setName("stop");
	this->addAttribute("offset", e.offset);
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->write();
}

void StreamWriter::visit(const RadialGradientElement& e) {
	this->setName("radialGradient");
	this->addGradientAttributes(e);
	if(e.cx.unit != Length::Unit_e::PERCENT || e.cx.value != 50){
		this->addAttribute("cx", e.cx);
	}
	if(e.cy.unit != Length::Unit_e::PERCENT || e.cy.value != 50){
		this->addAttribute("cy", e.cy);
	}
	if(e.r.unit != Length::Unit_e::PERCENT || e.r.value != 50){
		this->addAttribute("r", e.r);
	}
	if(e.fx.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("fx", e.fx);
	}
	if(e.fy.unit != Length::Unit_e::UNKNOWN){
		this->addAttribute("fy", e.fy);
	}
	this->write(&e);
}

void StreamWriter::visit(const LinearGradientElement& e) {
	this->setName("linearGradient");
	this->addGradientAttributes(e);
	if(e.x1.unit != Length::Unit_e::PERCENT || e.x1.value != 0){
		this->addAttribute("x1", e.x1);
	}
	if(e.y1.unit != Length::Unit_e::PERCENT || e.y1.value != 0){
		this->addAttribute("y1", e.y1);
	}
	if(e.x2.unit != Length::Unit_e::PERCENT || e.x2.value != 100){
		this->addAttribute("x2", e.x2);
	}
	if(e.y2.unit != Length::Unit_e::PERCENT || e.y2.value != 0){
		this->addAttribute("y2", e.y2);
	}
	this->write(&e);
}

void StreamWriter::visit(const DefsElement& e) {
	this->setName("defs");
	this->addElementAttributes(e);
	this->addTransformableAttributes(e);
	this->addStyleableAttributes(e);
	this->write(&e);
}

void StreamWriter::visit(const MaskElement& e) {
	this->setName("mask");
	this->addElementAttributes(e);
	this->addRectangleAttributes(e);
	this->addStyleableAttributes(e);
	
	if(e.maskUnits != CoordinateUnits_e::OBJECT_BOUNDING_BOX && e.maskUnits != CoordinateUnits_e::UNKNOWN){
		this->addAttribute("maskUnits", coordinateUnitsToString(e.maskUnits));
	}
	
	if(e.maskContentUnits != CoordinateUnits_e::USER_SPACE_ON_USE && e.maskContentUnits != CoordinateUnits_e::UNKNOWN){
		this->addAttribute("maskContentUnits", coordinateUnitsToString(e.maskContentUnits));
	}
	
	this->write(&e);
}

void StreamWriter::visit(const TextElement& e){
	this->setName("text");
	this->addElementAttributes(e);
	this->addTransformableAttributes(e);
	this->addStyleableAttributes(e);
	this->addTextPositioningAttributes(e);
	
	//TODO: add text element attributes
	
	this->write(&e);
}

void StreamWriter::visit(const SymbolElement& e) {
	this->setName("symbol");
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->addViewBoxedAttributes(e);
	this->addAspectRatioedAttributes(e);
	this->write(&e);
}

void StreamWriter::visit(const FilterElement& e){
	this->setName("filter");
	this->addElementAttributes(e);
	this->addStyleableAttributes(e);
	this->addRectangleAttributes(
			e,
			Rectangle(
				Length::make(-10, Length::Unit_e::PERCENT),
				Length::make(-10, Length::Unit_e::PERCENT),
				Length::make(120, Length::Unit_e::PERCENT),
				Length::make(120, Length::Unit_e::PERCENT)
			)
		);
	this->addReferencingAttributes(e);
	
	if(e.filterUnits != CoordinateUnits_e::UNKNOWN && e.filterUnits != CoordinateUnits_e::OBJECT_BOUNDING_BOX){
		this->addAttribute("filterUnits", coordinateUnitsToString(e.filterUnits));
	}

	if(e.primitiveUnits != CoordinateUnits_e::UNKNOWN && e.primitiveUnits != CoordinateUnits_e::USER_SPACE_ON_USE){
		this->addAttribute("primitiveUnits", coordinateUnitsToString(e.primitiveUnits));
	}
	
	this->write(&e);
}

void StreamWriter::visit(const FeGaussianBlurElement& e){
	this->setName("feGaussianBlur");
	
	this->addFilterPrimitiveAttributes(e);
	this->addInputableAttributes(e);
	
	if(e.isStdDeviationSpecified()){
		this->addAttribute("stdDeviation", numberOptionalNumberToString(e.stdDeviation, -1));
	}
	this->write();
}

void StreamWriter::visit(const FeColorMatrixElement& e){
	this->setName("feColorMatrix");
	
	this->addFilterPrimitiveAttributes(e);
	this->addInputableAttributes(e);
	
	{
		std::string typeValue;
		
		//write type
		switch(e.type){
			default:
			case FeColorMatrixElement::Type_e::MATRIX:
				//default value is 'matrix', so omit type attribute.
				break;
			case FeColorMatrixElement::Type_e::HUE_ROTATE:
				typeValue = "hueRotate";
				break;
			case FeColorMatrixElement::Type_e::SATURATE:
				typeValue = "saturate";
				break;
			case FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA:
				typeValue = "luminanceToAlpha";
				break;
		}
		if(typeValue.length() != 0){
			this->addAttribute("type", typeValue);
		}
	}
	
	{
		std::string valuesValue;
		switch(e.type){
			default:
			case FeColorMatrixElement::Type_e::MATRIX:
				//write 20 values
				{
					std::stringstream ss;
					for(unsigned i = 0; i != e.values.size(); ++i){
						if(i != 0){
							ss << " ";
						}
						ss << e.values[i];
					}
					valuesValue = ss.str();
				}
				break;
			case FeColorMatrixElement::Type_e::HUE_ROTATE:
				//fall-through
			case FeColorMatrixElement::Type_e::SATURATE:
				//write 1 value
				valuesValue = std::to_string(e.values[0]);
				break;
			case FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA:
				//'values' attribute can be omitted, so do nothing
				break;
		}
		if(valuesValue.length() != 0){
			this->addAttribute("values", valuesValue);
		}
	}
	
	this->write();
}


void StreamWriter::visit(const FeBlendElement& e){
	this->setName("feBlend");
	
	this->addFilterPrimitiveAttributes(e);
	this->addInputableAttributes(e);
	this->addSecondInputableAttributes(e);
	
	{
		std::string modeValue;
		switch(e.mode){
			default:
			case FeBlendElement::Mode_e::NORMAL:
				//default value, can be omitted
				break;
			case FeBlendElement::Mode_e::MULTIPLY:
				modeValue = "multiply";
				break;
			case FeBlendElement::Mode_e::SCREEN:
				modeValue = "screen";
				break;
			case FeBlendElement::Mode_e::DARKEN:
				modeValue = "darken";
				break;
			case FeBlendElement::Mode_e::LIGHTEN:
				modeValue = "lighten";
				break;
		}
		this->addAttribute("mode", modeValue);
	}
	
	this->write();
}

void StreamWriter::visit(const FeCompositeElement& e){
	this->setName("feComposite");
	
	this->addFilterPrimitiveAttributes(e);
	this->addInputableAttributes(e);
	this->addSecondInputableAttributes(e);
	
	{
		std::string operatorValue;
		switch(e.operator_v){
			default:
			case FeCompositeElement::Operator_e::OVER:
				//default value, can be omitted
				break;
			case FeCompositeElement::Operator_e::IN:
				operatorValue = "in";
				break;
			case FeCompositeElement::Operator_e::OUT:
				operatorValue = "out";
				break;
			case FeCompositeElement::Operator_e::ATOP:
				operatorValue = "atop";
				break;
			case FeCompositeElement::Operator_e::XOR:
				operatorValue = "xor";
				break;
			case FeCompositeElement::Operator_e::ARITHMETIC:
				operatorValue = "arithmetic";
				break;
		}
		if(operatorValue.length() != 0){
			this->addAttribute("operator", operatorValue);
		}
	}
	
	if(e.k1 != real(0)){
		this->addAttribute("k1", std::to_string(e.k1));
	}
	
	if(e.k2 != real(0)){
		this->addAttribute("k2", std::to_string(e.k2));
	}
	
	if(e.k3 != real(0)){
		this->addAttribute("k3", std::to_string(e.k3));
	}
	
	if(e.k4 != real(0)){
		this->addAttribute("k4", std::to_string(e.k4));
	}
	
	this->write();
}
