#pragma once

#include "visitor.hpp"

namespace svgdom{
/**
 * @brief clone visitor.
 * A visitor which allows cloning of Elements (and their children).
 */
class cloner : virtual public svgdom::const_visitor{
	svgdom::container root;
	svgdom::container* cur_parent = &root;

	void clone_children(const svgdom::container& e, svgdom::container& clone); 
	
public:
	/**
	* @brief Clone root element as T.
	* @return std::unique<T> where T is element type of root.
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
	
	void visit(const svgdom::path_element& e) override;
	void visit(const svgdom::rect_element& e) override;
	void visit(const svgdom::circle_element& e) override;
	void visit(const svgdom::ellipse_element& e) override;
	void visit(const svgdom::line_element& e) override;
	void visit(const svgdom::polyline_element& e) override;
	void visit(const svgdom::polygon_element& e) override;
	void visit(const svgdom::g_element& e) override;
	void visit(const svgdom::svg_element& e) override;
	void visit(const svgdom::symbol_element& e) override;
	void visit(const svgdom::use_element& e) override;
	void visit(const svgdom::defs_element& e) override;
	void visit(const svgdom::gradient::stop_element& e) override;
	void visit(const svgdom::linear_gradient_element& e) override;
	void visit(const svgdom::radial_gradient_element& e) override;
	void visit(const svgdom::filter_element& e) override;
	void visit(const svgdom::fe_gaussian_blur_element& e) override;
	void visit(const svgdom::image_element& e) override;
};

// TODO: DEPRECATED, remove.
typedef cloner Cloner;

}
