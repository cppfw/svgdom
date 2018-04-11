#include "Visitor.hpp"


using namespace svgdom;

void Visitor::visit(PathElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(RectElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(CircleElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(EllipseElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(LineElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(PolylineElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(PolygonElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(GElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(SvgElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(SymbolElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(UseElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(DefsElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(MaskElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(TextElement& e){
	this->defaultVisit(e, e);
}

void Visitor::visit(Gradient::StopElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(LinearGradientElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(RadialGradientElement& e) {
	this->defaultVisit(e, e);
}

void Visitor::visit(FilterElement& e){
	this->defaultVisit(e, e);
}

void Visitor::visit(FeGaussianBlurElement& e){
	this->defaultVisit(e);
}

void Visitor::visit(FeColorMatrixElement& e){
	this->defaultVisit(e);
}

void Visitor::visit(FeBlendElement& e){
	this->defaultVisit(e);
}

void Visitor::visit(FeCompositeElement& e){
	this->defaultVisit(e);
}

void Visitor::visit(ImageElement& e){
	this->defaultVisit(e);
}

void Visitor::defaultVisit(Element& e, Container& c) {
	this->defaultVisit(e);
	this->relayAccept(c);
}


void Visitor::relayAccept(Container& container){
	auto oldParent = this->curParent_v;
	auto oldIter = this->curIter_v;
	this->curParent_v = &container;
	for(auto i = container.children.begin(), e = container.children.end(); i != e; ++i){
		this->curIter_v = i;
		(*i)->accept(*this);
	}
	this->curIter_v = oldIter;
	this->curParent_v = oldParent;
}

void ConstVisitor::visit(const PathElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const RectElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const CircleElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const EllipseElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const LineElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const PolylineElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const PolygonElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const GElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const SvgElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const SymbolElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const UseElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const DefsElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const MaskElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const TextElement& e){
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const Gradient::StopElement& e) {
	this->defaultVisit(e);
}

void ConstVisitor::visit(const LinearGradientElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const RadialGradientElement& e) {
	this->defaultVisit(e, e);
}

void ConstVisitor::visit(const FilterElement& e){
	this->defaultVisit(e ,e);
}

void ConstVisitor::visit(const FeGaussianBlurElement& e){
	this->defaultVisit(e);
}

void ConstVisitor::visit(const FeColorMatrixElement& e){
	this->defaultVisit(e);
}

void ConstVisitor::visit(const FeBlendElement& e){
	this->defaultVisit(e);
}

void ConstVisitor::visit(const FeCompositeElement& e){
	this->defaultVisit(e);
}

void ConstVisitor::visit(const ImageElement& e){
	this->defaultVisit(e);
}

void ConstVisitor::defaultVisit(const Element& e, const Container& c) {
	this->defaultVisit(e);
	this->relayAccept(c);
}

void ConstVisitor::relayAccept(const Container& container){
	for(auto& e : container.children){
		e->accept(*this);
	}
}
