#include "Cloner.hpp"

using namespace svgdom;

void Cloner::cloneChildren(const Container& e, Container& clone){
	auto oldParent = curParent;
	curParent = &clone;
	relayAccept(e);
	curParent = oldParent;
}

void Cloner::visit(const GElement& e) {
	auto clone = utki::makeUnique<GElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const SymbolElement& e) {
	auto clone = utki::makeUnique<SymbolElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const SvgElement& e) {
	auto clone = utki::makeUnique<SvgElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const RadialGradientElement& e) {
	auto clone = utki::makeUnique<RadialGradientElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const LinearGradientElement& e) {
	auto clone = utki::makeUnique<LinearGradientElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const DefsElement& e) {
	auto clone = utki::makeUnique<DefsElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const FilterElement& e) {
	auto clone = utki::makeUnique<FilterElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}


void Cloner::visit(const PolylineElement& e) {
	auto clone = utki::makeUnique<PolylineElement>(e);
	curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const CircleElement& e) {
	auto clone = utki::makeUnique<CircleElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const UseElement& e) {
	auto clone = utki::makeUnique<UseElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const Gradient::StopElement& e) {
	auto clone = utki::makeUnique<Gradient::StopElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const PathElement& e) {
	auto clone = utki::makeUnique<PathElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const RectElement& e) {
	auto clone = utki::makeUnique<RectElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const LineElement& e) {
	auto clone = utki::makeUnique<LineElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const EllipseElement& e) {
	auto clone = utki::makeUnique<EllipseElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const PolygonElement& e) {
	auto clone = utki::makeUnique<PolygonElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const FeGaussianBlurElement& e) {
	auto clone = utki::makeUnique<FeGaussianBlurElement>(e);
    curParent->children.push_back(std::move(clone));
}

void Cloner::visit(const ImageElement& e) {
	auto clone = utki::makeUnique<ImageElement>(e);
    curParent->children.push_back(std::move(clone));
}
