#include "visitor.hpp"

using namespace svgdom;

void visitor::visit(path_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(rect_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(circle_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(ellipse_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(line_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(polyline_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(polygon_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(GElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(SvgElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(SymbolElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(UseElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(DefsElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(MaskElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(TextElement& e){
	this->defaultVisit(e, e);
}

void visitor::visit(gradient::stop_element& e) {
	this->defaultVisit(e);
}

void visitor::visit(linear_gradient_element& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(radial_gradient_element& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(FilterElement& e){
	this->defaultVisit(e, e);
}

void visitor::visit(FeGaussianBlurElement& e){
	this->defaultVisit(e);
}

void visitor::visit(FeColorMatrixElement& e){
	this->defaultVisit(e);
}

void visitor::visit(FeBlendElement& e){
	this->defaultVisit(e);
}

void visitor::visit(FeCompositeElement& e){
	this->defaultVisit(e);
}

void visitor::visit(image_element& e){
	this->defaultVisit(e);
}

void visitor::default_visit(element& e, container& c) {
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
	this->defaultVisit(e);
}

void const_visitor::visit(const rect_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const circle_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const ellipse_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const line_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const polyline_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const polygon_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const GElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const SvgElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const SymbolElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const UseElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const DefsElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const MaskElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const TextElement& e){
	this->defaultVisit(e, e);
}

void const_visitor::visit(const gradient::stop_element& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const linear_gradient_element& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const radial_gradient_element& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const FilterElement& e){
	this->defaultVisit(e ,e);
}

void const_visitor::visit(const FeGaussianBlurElement& e){
	this->defaultVisit(e);
}

void const_visitor::visit(const FeColorMatrixElement& e){
	this->defaultVisit(e);
}

void const_visitor::visit(const FeBlendElement& e){
	this->defaultVisit(e);
}

void const_visitor::visit(const FeCompositeElement& e){
	this->defaultVisit(e);
}

void const_visitor::visit(const image_element& e){
	this->defaultVisit(e);
}

void const_visitor::default_visit(const element& e, const container& c) {
	this->default_visit(e);
	this->relay_accept(c);
}

void const_visitor::relay_accept(const container& c){
	for(auto& e : c.children){
		e->accept(*this);
	}
}
