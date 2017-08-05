#pragma once

#include "Visitor.hpp"

namespace svgdom{

class StreamWriter : public Visitor{
	std::ostream& s;
	
	unsigned indent = 0;
	
	void childrenToStream(const Container& e);
	
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
	void visit(const CustomStringElement& e) override;
};

}
