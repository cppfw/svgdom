#include "Visitor.hpp"


using namespace svgdom;

void Visitor::visit(const PathElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const RectElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const CircleElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const EllipseElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const LineElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const PolylineElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const PolygonElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const GElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const SvgElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const SymbolElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const UseElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const DefsElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const Gradient::StopElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const LinearGradientElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const RadialGradientElement& e) {
	this->defaultVisit(e);
}

void Visitor::visit(const FilterElement& e){
	this->defaultVisit(e);
}

void Visitor::visit(const FeGaussianBlurElement& e){
	this->defaultVisit(e);
}
