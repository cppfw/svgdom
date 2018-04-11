#pragma once

#include "Visitor.hpp"

namespace svgdom{

class StreamWriter : virtual public ConstVisitor{
private:
	void childrenToStream(const Container& e);
	
	std::string name;
	std::vector<std::pair<std::string, std::string>> attributes;
protected:
	// s, indent, and indentStr() are made protected to allow writing arbitrary content to stream for those who extend the class, as this was needed in some projects.
	std::ostream& s;
	unsigned indent = 0;
	std::string indentStr();
	
	void setName(const std::string& name);
	void addAttribute(const std::string& name, const std::string& value);
	void addAttribute(const std::string& name, const Length& value);
	void addAttribute(const std::string& name, real value);
	void write(const Container* children = nullptr);
	
	void addElementAttributes(const Element& e);
	void addTransformableAttributes(const Transformable& e);
	void addStyleableAttributes(const Styleable& e);
	void addViewBoxedAttributes(const ViewBoxed& e);
	void addAspectRatioedAttributes(const AspectRatioed& e);
	void addRectangleAttributes(
			const Rectangle& e,
			const Rectangle& defaultValues = Rectangle(
					Length::make(0, Length::Unit_e::PERCENT),
					Length::make(0, Length::Unit_e::PERCENT),
					Length::make(100, Length::Unit_e::PERCENT),
					Length::make(100, Length::Unit_e::PERCENT)
				)
		);
	void addShapeAttributes(const Shape& e);
	void addReferencingAttributes(const Referencing& e);
	void addGradientAttributes(const Gradient& e);
	void addFilterPrimitiveAttributes(const FilterPrimitive& e);
	void addInputableAttributes(const Inputable& e);
	void addSecondInputableAttributes(const SecondInputable& e);
	void addTextPositioningAttributes(const TextPositioning& e);
	
public:
	StreamWriter(std::ostream& s) : s(s) {}
	
	void visit(const GElement& e) override;
	void visit(const SvgElement& e) override;
	void visit(const SymbolElement& e) override;
	void visit(const DefsElement& e) override;
	void visit(const LinearGradientElement& e) override;
	void visit(const RadialGradientElement& e) override;
	void visit(const Gradient::StopElement& e) override;
	void visit(const UseElement& e) override;
	void visit(const PathElement& e) override;
	void visit(const CircleElement& e) override;
	void visit(const PolylineElement& e) override;
	void visit(const PolygonElement& e) override;
	void visit(const EllipseElement& e) override;
	void visit(const RectElement& e) override;
	void visit(const LineElement& e) override;
	void visit(const FilterElement& e) override;
	void visit(const FeGaussianBlurElement& e) override;
	void visit(const FeColorMatrixElement& e) override;
	void visit(const FeBlendElement& e) override;
	void visit(const FeCompositeElement& e) override;
	void visit(const ImageElement& e) override;
	void visit(const MaskElement& e) override;
	void visit(const TextElement& e) override;

};

}
