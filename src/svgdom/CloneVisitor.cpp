#include <CloneVisitor.hpp>

using namespace svgdom;
using namespace std;

void CloneVisitor::cloneChildren(const Container& e, Container& clone){
	auto oldParent = curParent;
	curParent = &clone;
	relayAccept(e);
	curParent = oldParent;
}

void CloneVisitor::visit(const GElement& e) {
	auto clone = utki::makeUnique<GElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const SymbolElement& e) {
	auto clone = utki::makeUnique<SymbolElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const SvgElement& e) {
	auto clone = utki::makeUnique<SvgElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const RadialGradientElement& e) {
	auto clone = utki::makeUnique<RadialGradientElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const LinearGradientElement& e) {
	auto clone = utki::makeUnique<LinearGradientElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const DefsElement& e) {
	auto clone = utki::makeUnique<DefsElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const FilterElement& e) {
	auto clone = utki::makeUnique<FilterElement>(e);
	cloneChildren(e, *clone); 
	curParent->children.push_back(std::move(clone));
}


void CloneVisitor::visit(const PolylineElement& e) {
	auto clone = utki::makeUnique<PolylineElement>(e);
	curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const CircleElement& e) {
	auto clone = utki::makeUnique<CircleElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const UseElement& e) {
	auto clone = utki::makeUnique<UseElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const Gradient::StopElement& e) {
	auto clone = utki::makeUnique<Gradient::StopElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const PathElement& e) {
	auto clone = utki::makeUnique<PathElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const RectElement& e) {
	auto clone = utki::makeUnique<RectElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const LineElement& e) {
	auto clone = utki::makeUnique<LineElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const EllipseElement& e) {
	auto clone = utki::makeUnique<EllipseElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const PolygonElement& e) {
	auto clone = utki::makeUnique<PolygonElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const FeGaussianBlurElement& e) {
	auto clone = utki::makeUnique<FeGaussianBlurElement>(e);
    curParent->children.push_back(std::move(clone));
}

void CloneVisitor::visit(const ImageElement& e) {
	auto clone = utki::makeUnique<ImageElement>(e);
    curParent->children.push_back(std::move(clone));
}
