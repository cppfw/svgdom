#pragma once

#include "elements/Structurals.hpp"
#include "elements/Shapes.hpp"
#include "elements/Gradients.hpp"
#include "elements/Filter.hpp"
#include "elements/ImageElement.hpp"
#include "elements/TextElement.hpp"

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
protected:
	/**
	 * @brief Type of child element iterator into the parent Container.
	 */
	typedef decltype(Container::children)::iterator T_ChildIter;
private:
	Container* curParent_v = nullptr;
	T_ChildIter curIter_v;
	
protected:
	/**
	 * @brief Get current Container whose children are being visited.
	 * @return Pointer to current traversed Container.
	 * @return nullptr if root SVG element is being visited.
	 */
	decltype(curParent_v) curParent()const{
		return this->curParent_v;
	}
	
	/**
	 * @brief Get iterator if current visited child element.
	 * Returns iterator into the parent Container of the currently visited child element.
	 * Note, that removing the visited element from its parent during the element is visited will
	 * lead to undefined behavior. Instead, one should store the iterators until the whole SVG tree
	 * traversing is completed and only then perform elements removal if needed.
	 * @return Iterator of currently visited child element.
	 */
	T_ChildIter curIter()const{
		return this->curIter_v;
	}
	
	/**
	 * @brief Relay accept to children.
	 * @param container - container to whose children the 'accept' should be relayed.
	 */
	void relayAccept(Container& container);
	
public:
	virtual void visit(PathElement& e);
	virtual void visit(RectElement& e);
	virtual void visit(CircleElement& e);
	virtual void visit(EllipseElement& e);
	virtual void visit(LineElement& e);
	virtual void visit(PolylineElement& e);
	virtual void visit(PolygonElement& e);
	virtual void visit(GElement& e);
	virtual void visit(SvgElement& e);
	virtual void visit(SymbolElement& e);
	virtual void visit(UseElement& e);
	virtual void visit(DefsElement& e);
	virtual void visit(Gradient::StopElement& e);
	virtual void visit(LinearGradientElement& e);
	virtual void visit(RadialGradientElement& e);
	virtual void visit(FilterElement& e);
	virtual void visit(FeGaussianBlurElement& e);
	virtual void visit(FeColorMatrixElement& e);
	virtual void visit(FeBlendElement& e);
	virtual void visit(FeCompositeElement& e);
	virtual void visit(ImageElement& e);
	virtual void visit(MaskElement& e);
	virtual void visit(TextElement& e);
	
	/**
	 * @brief Default visit method.
	 * This method is called by all the visit methods by default.
	 * @param e - SVG element to visit.
	 */
	virtual void defaultVisit(Element& e){}
	
	/**
	 * @brief Default visit method for container elements.
	 * Default implementation of this method calls this->defaultVisit(e) and
	 * then calls this->relayAccept(c);
	 * @param e - element to visit.
	 * @param c - 'Container' ancestor of the element to visit.
	 */
	virtual void defaultVisit(Element& e, Container& c);
	
	virtual ~Visitor()noexcept{}
};

/**
 * @brief Constant version of Visitor.
 * Same as Visitor, but it takes all elements as 'const' arguments, so it cannot modify elements.
 */
class ConstVisitor{
protected:
	
	/**
	 * @brief Relay accept to children.
	 * @param container - container to whose children the 'accept' should be relayed.
	 */
	void relayAccept(const Container& container);
	
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
	virtual void visit(const FeColorMatrixElement& e);
	virtual void visit(const FeBlendElement& e);
	virtual void visit(const FeCompositeElement& e);
	virtual void visit(const ImageElement& e);
	virtual void visit(const MaskElement& e);
	virtual void visit(const TextElement& e);
	
	/**
	 * @brief Default visit method.
	 * This method is called by all the visit methods by default.
	 * @param e - SVG element to visit.
	 */
	virtual void defaultVisit(const Element& e){}
	
	/**
	 * @brief Default visit method for container elements.
	 * Default implementation of this method calls this->defaultVisit(e) and
	 * then calls this->relayAccept(c);
	 * @param e - element to visit.
	 * @param c - 'Container' ancestor of the element to visit.
	 */
	virtual void defaultVisit(const Element& e, const Container& c);
	
	virtual ~ConstVisitor()noexcept{}
};

}
