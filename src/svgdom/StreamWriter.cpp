#include "StreamWriter.hpp"

#include <utki/util.hpp>

#include "util.hxx"

using namespace svgdom;

std::string StreamWriter::indentStr() {
	std::string ind;

	std::stringstream ss;
	for(unsigned i = 0; i != this->indent; ++i){
		ss << "\t";
	}
	return ss.str();
}


void StreamWriter::childrenToStream(const Container& e) {
	++this->indent;
	utki::ScopeExit scopeExit([this]() {
		--this->indent;
	});
	for (auto& c : e.children) {
		c->accept(*this);
	}
}


void StreamWriter::visit(const GElement& e) {
	auto ind = indentStr();

	this->s << ind << "<g";
	e.attribsToStream(this->s);

	if (e.children.size() == 0) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		this->childrenToStream(e);
		this->s << ind << "</g>";
	}
	this->s << std::endl;
}

void StreamWriter::visit(const SvgElement& e) {
	auto ind = indentStr();

	this->s << ind << "<svg";
	if (this->indent == 0) {//if outermost "svg" element
		this->s << " xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"";
	}
	e.attribsToStream(this->s);

	if (e.children.size() == 0) {
		s << "/>";
	} else {
		s << ">" << std::endl;
		this->childrenToStream(e);
		s << ind << "</svg>";
	}
	s << std::endl;
}

void StreamWriter::visit(const LineElement& e) {
	auto ind = indentStr();

	s << ind << "<line";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const RectElement& e) {
	auto ind = indentStr();

	s << ind << "<rect";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const EllipseElement& e) {
	auto ind = indentStr();

	s << ind << "<ellipse";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const PolygonElement& e) {
	auto ind = indentStr();

	s << ind << "<polygon";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const PolylineElement& e) {
	auto ind = indentStr();

	s << ind << "<polyline";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const CircleElement& e) {
	auto ind = indentStr();

	s << ind << "<circle";
	e.attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const PathElement& e) {
	auto ind = indentStr();

	s << ind << "<path";

	e.attribsToStream(s);

	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const UseElement& e) {
	auto ind = indentStr();

	s << ind << "<use";
	e.Element::attribsToStream(s);
	e.Transformable::attribsToStream(s);
	e.Styleable::attribsToStream(s);
	e.Rectangle::attribsToStream(s);
	e.Referencing::attribsToStream(s);
	s << "/>";
	s << std::endl;
}

void StreamWriter::visit(const Gradient::StopElement& e) {
	auto ind = indentStr();
	this->s << ind << "<stop";
	e.attribsToStream(this->s);
	this->s << "/>" << std::endl;
}

void StreamWriter::visit(const RadialGradientElement& e) {
	auto ind = indentStr();

	this->s << ind << "<radialGradient";
	e.attribsToStream(this->s);

	if (e.children.size() == 0) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		this->childrenToStream(e);
		this->s << ind << "</radialGradient>";
	}
	this->s << std::endl;
}

void StreamWriter::visit(const LinearGradientElement& e) {
	auto ind = indentStr();

	this->s << ind << "<linearGradient";
	e.attribsToStream(this->s);

	if (e.children.size() == 0) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		this->childrenToStream(e);
		this->s << ind << "</linearGradient>";
	}
	this->s << std::endl;
}

void StreamWriter::visit(const DefsElement& e) {
	auto ind = indentStr();

	this->s << ind << "<defs";
	e.Element::attribsToStream(s);
	e.Transformable::attribsToStream(s);
	e.Styleable::attribsToStream(s);

	if (e.children.size() == 0) {
		s << "/>";
	} else {
		s << ">" << std::endl;
		this->childrenToStream(e);
		s << ind << "</defs>";
	}
	s << std::endl;
}

void StreamWriter::visit(const SymbolElement& e) {
	auto ind = indentStr();

	this->s << ind << "<symbol";

	e.attribsToStream(this->s);

	if (e.children.size() == 0) {
		s << "/>";
	} else {
		s << ">" << std::endl;
		this->childrenToStream(e);
		s << ind << "</symbol>";
	}
	s << std::endl;
}
