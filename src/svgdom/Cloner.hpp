#pragma once

#include "Visitor.hpp"

namespace svgdom{
/**
 * @brief clone visitor.
 * A visitor which allows cloning of Elements (abd their children).
 */
class Cloner : virtual public svgdom::ConstVisitor {
	svgdom::Container root;
	svgdom::Container* curParent = &root;

	void cloneChildren(const svgdom::Container& e, svgdom::Container& clone); 
	
public:
	/**
	* @brief Clone root element as T.
	* @return std::unique<T> where T is Element type of root.
	*/
	template <class T> std::unique_ptr<T> getCloneAs() {
		if (root.children.size() != 1) {
			return nullptr;
		}
		auto ret = std::unique_ptr<T>(dynamic_cast<T*>(root.children.back().get()));
	  if(ret){
			root.children.back().release();
	  }
		root.children.clear();
		return ret;
	}
	
	void visit(const svgdom::PathElement& e) override;
  void visit(const svgdom::RectElement& e) override;
  void visit(const svgdom::CircleElement& e) override;
  void visit(const svgdom::EllipseElement& e) override;
  void visit(const svgdom::LineElement& e) override;
  void visit(const svgdom::PolylineElement& e) override;
  void visit(const svgdom::PolygonElement& e) override;
  void visit(const svgdom::GElement& e) override;
  void visit(const svgdom::SvgElement& e) override;
  void visit(const svgdom::SymbolElement& e) override;
  void visit(const svgdom::UseElement& e) override;
  void visit(const svgdom::DefsElement& e) override;
  void visit(const svgdom::Gradient::StopElement& e) override;
  void visit(const svgdom::LinearGradientElement& e) override;
  void visit(const svgdom::RadialGradientElement& e) override;
  void visit(const svgdom::FilterElement& e) override;
  void visit(const svgdom::FeGaussianBlurElement& e) override;
	void visit(const svgdom::ImageElement& e) override;
};

}
