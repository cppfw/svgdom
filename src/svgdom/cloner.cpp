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

void cloner::visit(const RadialGradientElement& e) {
	auto clone = std::make_unique<RadialGradientElement>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const LinearGradientElement& e) {
	auto clone = std::make_unique<LinearGradientElement>(e);
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


void cloner::visit(const PolylineElement& e) {
	auto clone = std::make_unique<PolylineElement>(e);
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const CircleElement& e) {
	auto clone = std::make_unique<CircleElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const UseElement& e) {
	auto clone = std::make_unique<UseElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const Gradient::StopElement& e) {
	auto clone = std::make_unique<Gradient::StopElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const PathElement& e) {
	auto clone = std::make_unique<PathElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const RectElement& e) {
	auto clone = std::make_unique<RectElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const LineElement& e) {
	auto clone = std::make_unique<LineElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const EllipseElement& e) {
	auto clone = std::make_unique<EllipseElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const PolygonElement& e) {
	auto clone = std::make_unique<PolygonElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const FeGaussianBlurElement& e) {
	auto clone = std::make_unique<FeGaussianBlurElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const ImageElement& e) {
	auto clone = std::make_unique<ImageElement>(e);
    this->cur_parent->children.push_back(std::move(clone));
}
