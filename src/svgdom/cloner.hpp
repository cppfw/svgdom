#pragma once

#include "visitor.hpp"

namespace svgdom{
/**
 * @brief clone visitor.
 * A visitor which allows cloning of Elements (and their children).
 */
class cloner : virtual public svgdom::ConstVisitor{
	svgdom::container root;
	svgdom::container* cur_parent = &root;

	void clone_children(const svgdom::container& e, svgdom::container& clone); 
	
public:
	/**
	* @brief Clone root element as T.
	* @return std::unique<T> where T is Element type of root.
	*/
	template <class T> std::unique_ptr<T> get_clone_as() {
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

	// TODO: DEPRECATED, remove.
	template <class T> std::unique_ptr<T> getCloneAs(){
		return this->get_clone_as<T>();
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
	void visit(const svgdom::gradient::stop_element& e) override;
	void visit(const svgdom::LinearGradientElement& e) override;
	void visit(const svgdom::RadialGradientElement& e) override;
	void visit(const svgdom::FilterElement& e) override;
	void visit(const svgdom::FeGaussianBlurElement& e) override;
	void visit(const svgdom::ImageElement& e) override;
};

// TODO: DEPRECATED, remove.
typedef cloner Cloner;

}
