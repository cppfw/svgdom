#pragma once

#include "Visitor.hpp"

namespace svgdom{

class StreamWriter : public Visitor{
private:
	void childrenToStream(const Container& e);
	
	std::string name;
	std::vector<std::pair<std::string, std::string>> attributes;
protected:
	std::ostream& s;
	std::string indentStr();
	unsigned indent = 0;
	
	void setName(const std::string& name);
	void addAttribute(const std::string& name, const std::string& value);
	void addAttribute(const std::string& name, const Length& value);
	void addAttribute(const std::string& name, real value);
	void write(const Container* children = nullptr);
	
	void addElementAttributes(const Element& e);
	void addTransformableAttributes(const Transformable& e);
	void addStyleableAttributes(const Styleable& e);
	void addViewBoxedAttributes(const ViewBoxed& e);
	void addRectangleAttributes(const Rectangle& e);
	void addShapeAttributes(const Shape& e);
	void addReferencingAttributes(const Referencing& e);
	void addGradientAttributes(const Gradient& e);
	
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
};

}
