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

#include "visitor.hpp"

using namespace svgdom;

void visitor::visit(path_element& e){
	this->default_visit(e);
}

void visitor::visit(rect_element& e){
	this->default_visit(e);
}

void visitor::visit(circle_element& e){
	this->default_visit(e);
}

void visitor::visit(ellipse_element& e){
	this->default_visit(e);
}

void visitor::visit(line_element& e){
	this->default_visit(e);
}

void visitor::visit(polyline_element& e){
	this->default_visit(e);
}

void visitor::visit(polygon_element& e){
	this->default_visit(e);
}

void visitor::visit(g_element& e){
	this->default_visit(e, e);
}

void visitor::visit(svg_element& e){
	this->default_visit(e, e);
}

void visitor::visit(symbol_element& e){
	this->default_visit(e, e);
}

void visitor::visit(use_element& e){
	this->default_visit(e);
}

void visitor::visit(defs_element& e){
	this->default_visit(e, e);
}

void visitor::visit(mask_element& e){
	this->default_visit(e, e);
}

void visitor::visit(text_element& e){
	this->default_visit(e, e);
}

void visitor::visit(style_element& e){
	this->default_visit(e);
}

void visitor::visit(gradient::stop_element& e){
	this->default_visit(e);
}

void visitor::visit(linear_gradient_element& e){
	this->default_visit(e, e);
}

void visitor::visit(radial_gradient_element& e){
	this->default_visit(e, e);
}

void visitor::visit(filter_element& e){
	this->default_visit(e, e);
}

void visitor::visit(fe_gaussian_blur_element& e){
	this->default_visit(e);
}

void visitor::visit(fe_color_matrix_element& e){
	this->default_visit(e);
}

void visitor::visit(fe_blend_element& e){
	this->default_visit(e);
}

void visitor::visit(fe_composite_element& e){
	this->default_visit(e);
}

void visitor::visit(image_element& e){
	this->default_visit(e);
}

void visitor::default_visit(element& e, container& c){
	this->default_visit(e);
	this->relay_accept(c);
}

void visitor::relay_accept(container& c){
	auto oldParent = this->cur_parent_container;
	auto oldIter = this->cur_iterator;
	this->cur_parent_container = &c;
	for(auto i = c.children.begin(), e = c.children.end(); i != e; ++i){
		this->cur_iterator = i;
		(*i)->accept(*this);
	}
	this->cur_iterator = oldIter;
	this->cur_parent_container = oldParent;
}

void const_visitor::visit(const path_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const rect_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const circle_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const ellipse_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const line_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const polyline_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const polygon_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const g_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const svg_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const symbol_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const use_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const defs_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const mask_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const text_element& e){
	this->default_visit(e, e);
}

void const_visitor::visit(const style_element& e){
	this->default_visit(e);
}

void const_visitor::visit(const gradient::stop_element& e) {
	this->default_visit(e);
}

void const_visitor::visit(const linear_gradient_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const radial_gradient_element& e) {
	this->default_visit(e, e);
}

void const_visitor::visit(const filter_element& e){
	this->default_visit(e ,e);
}

void const_visitor::visit(const fe_gaussian_blur_element& e){
	this->default_visit(e);
}

void const_visitor::visit(const fe_color_matrix_element& e){
	this->default_visit(e);
}

void const_visitor::visit(const fe_blend_element& e){
	this->default_visit(e);
}

void const_visitor::visit(const fe_composite_element& e){
	this->default_visit(e);
}

void const_visitor::visit(const image_element& e){
	this->default_visit(e);
}

void const_visitor::default_visit(const element& e, const container& c){
	this->default_visit(e);
	this->relay_accept(c);
}

void const_visitor::relay_accept(const container& c){
	for(auto& e : c.children){
		e->accept(*this);
	}
}
