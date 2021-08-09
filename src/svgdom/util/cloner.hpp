/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#pragma once

#include "../visitor.hpp"

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
	template <class T> std::unique_ptr<T> get_clone_as(){
		if(root.children.size() != 1){
			return nullptr;
		}
		auto ret = std::unique_ptr<T>(dynamic_cast<T*>(root.children.back().get()));
		if(ret){
			root.children.back().release();
		}
		root.children.clear();
		return ret;
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
	void visit(const svgdom::style_element& e) override;
};

}
