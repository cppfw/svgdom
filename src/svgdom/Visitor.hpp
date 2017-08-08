#pragma once

#include "elements/Structurals.hpp"
#include "elements/Shapes.hpp"
#include "elements/Gradients.hpp"
#include "elements/Filter.hpp"

namespace svgdom{
/**
 * @brief Visitor interface.
 * A visitor interface which allows traversing of the SVG element tree.
 * It utilizes the 'visitor' pattern.
 * Each Element-based class can override the 'accept' method which will call
 * corresponding 'visit' method from Visitor. And user can override Visitor's methods
 * to implement their own operation to perform on each SVG element.
 */
class Visitor{
public:
	virtual void visit(const PathElement& e);
	virtual void visit(const RectElement& e);
	virtual void visit(const CircleElement& e);
	virtual void visit(const EllipseElement& e);
	virtual void visit(const LineElement& e);
	virtual void visit(const PolylineElement& e);
	virtual void visit(const PolygonElement& e);
	virtual void visit(const GElement& e);
	virtual void visit(const SvgElement& e);
	virtual void visit(const SymbolElement& e);
	virtual void visit(const UseElement& e);
	virtual void visit(const DefsElement& e);
	virtual void visit(const Gradient::StopElement& e);
	virtual void visit(const LinearGradientElement& e);
	virtual void visit(const RadialGradientElement& e);
	virtual void visit(const FilterElement& e);
	virtual void visit(const FeGaussianBlurElement& e);
	
	/**
	 * @brief Default visit method.
	 * This method is called by all the visit methods by default.
	 * @param e - SVG element to visit.
	 */
	virtual void defaultVisit(const Element& e){}
	
	virtual ~Visitor()noexcept{}
};

}
