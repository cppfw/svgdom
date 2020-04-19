#include "visitor.hpp"

using namespace svgdom;

void visitor::visit(PathElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(RectElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(CircleElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(EllipseElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(LineElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(PolylineElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(PolygonElement& e) {
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

void visitor::visit(Gradient::StopElement& e) {
	this->defaultVisit(e);
}

void visitor::visit(LinearGradientElement& e) {
	this->defaultVisit(e, e);
}

void visitor::visit(RadialGradientElement& e) {
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

void visitor::visit(ImageElement& e){
	this->defaultVisit(e);
}

void visitor::default_visit(Element& e, container& c) {
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

void const_visitor::visit(const PathElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const RectElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const CircleElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const EllipseElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const LineElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const PolylineElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const PolygonElement& e) {
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

void const_visitor::visit(const Gradient::StopElement& e) {
	this->defaultVisit(e);
}

void const_visitor::visit(const LinearGradientElement& e) {
	this->defaultVisit(e, e);
}

void const_visitor::visit(const RadialGradientElement& e) {
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

void const_visitor::visit(const ImageElement& e){
	this->defaultVisit(e);
}

void const_visitor::default_visit(const Element& e, const container& c) {
	this->default_visit(e);
	this->relay_accept(c);
}

void const_visitor::relay_accept(const container& c){
	for(auto& e : c.children){
		e->accept(*this);
	}
}
