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

void visitor::visit(FeBlendElement& e){
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
	auto oldParent = this->curParent_v;
	auto oldIter = this->curIter_v;
	this->curParent_v = &c;
	for(auto i = c.children.begin(), e = c.children.end(); i != e; ++i){
		this->curIter_v = i;
		(*i)->accept(*this);
	}
	this->curIter_v = oldIter;
	this->curParent_v = oldParent;
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
