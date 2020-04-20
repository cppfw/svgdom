#include "cloner.hpp"

using namespace svgdom;

void cloner::clone_children(const container& e, container& clone){
	auto oldParent = this->cur_parent;
	this->cur_parent = &clone;
	relayAccept(e);
	this->cur_parent = oldParent;
}

void cloner::visit(const GElement& e) {
	auto clone = std::make_unique<GElement>(e);
	this->clone_children(e, *clone);
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const SymbolElement& e) {
	auto clone = std::make_unique<SymbolElement>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const SvgElement& e) {
	auto clone = std::make_unique<SvgElement>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const radial_gradient_element& e) {
	auto clone = std::make_unique<radial_gradient_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const linear_gradient_element& e) {
	auto clone = std::make_unique<linear_gradient_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const DefsElement& e) {
	auto clone = std::make_unique<DefsElement>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const FilterElement& e) {
	auto clone = std::make_unique<FilterElement>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}


void cloner::visit(const polyline_element& e) {
	auto clone = std::make_unique<polyline_element>(e);
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const circle_element& e) {
	auto clone = std::make_unique<circle_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const UseElement& e) {
	auto clone = std::make_unique<UseElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const gradient::stop_element& e) {
	auto clone = std::make_unique<gradient::stop_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const path_element& e) {
	auto clone = std::make_unique<path_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const rect_element& e) {
	auto clone = std::make_unique<rect_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const line_element& e) {
	auto clone = std::make_unique<line_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const ellipse_element& e) {
	auto clone = std::make_unique<ellipse_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const polygon_element& e) {
	auto clone = std::make_unique<polygon_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const FeGaussianBlurElement& e) {
	auto clone = std::make_unique<FeGaussianBlurElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const image_element& e) {
	auto clone = std::make_unique<image_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}
