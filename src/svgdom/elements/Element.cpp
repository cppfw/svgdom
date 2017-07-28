#include "Element.hpp"

#include <ostream>
#include <sstream>

#include <utki/util.hpp>

#include "Container.hpp"
#include "../Visitor.hpp"
#include "../util.hxx"

using namespace svgdom;


void Element::attribsToStream(std::ostream& s) const{
	if(this->id.length() != 0){
		s << " id=\"" << this->id << "\"";
	}
}

namespace{
struct StreamWriter : public Visitor{
	std::ostream& s;
	
	unsigned indent = 0;
	
	StreamWriter(std::ostream& s) : s(s) {}
	
	void childrenToStream(const Container& e){
		++this->indent;
		utki::ScopeExit scopeExit([this](){--this->indent;});
		for(auto& c : e.children){
			c->accept(*this);
		}
	}
	
	void visit(const GElement& e) override{
		auto ind = indentStr(this->indent);
	
		this->s << ind << "<g";
		e.attribsToStream(this->s);

		if(e.children.size() == 0){
			this->s << "/>";
		}else{
			this->s << ">" << std::endl;
			this->childrenToStream(e);
			this->s << ind << "</g>";
		}
		this->s << std::endl;
	}
	
	void visit(const SvgElement& e) override{
		auto ind = indentStr(this->indent);
	
		this->s << ind << "<svg";
		if(this->indent == 0){//if outermost "svg" element
			this->s << " xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"";
		}
		e.attribsToStream(this->s);

		if(e.children.size() == 0){
			s << "/>";
		}else{
			s << ">" << std::endl;
			this->childrenToStream(e);
			s << ind << "</svg>";
		}
		s << std::endl;
	}

	void visit(const SymbolElement& e) override{
		auto ind = indentStr(this->indent);

		this->s << ind << "<symbol";

		e.attribsToStream(this->s);

		if (e.children.size() == 0) {
			s << "/>";
		}else{
			s << ">" << std::endl;
			this->childrenToStream(e);
			s << ind << "</symbol>";
		}
		s << std::endl;
	}

	void visit(const DefsElement& e) override{
		auto ind = indentStr(this->indent);

		this->s << ind << "<defs";
		e.Element::attribsToStream(s);
		e.Transformable::attribsToStream(s);
		e.Styleable::attribsToStream(s);

		if(e.children.size() == 0){
			s << "/>";
		}else{
			s << ">" << std::endl;
			this->childrenToStream(e);
			s << ind << "</defs>";
		}
		s << std::endl;
	}

	void visit(const LinearGradientElement& e) override{
		auto ind = indentStr(this->indent);
	
		this->s << ind << "<linearGradient";
		e.attribsToStream(this->s);

		if(e.children.size() == 0){
			this->s << "/>";
		}else{
			this->s << ">" << std::endl;
			this->childrenToStream(e);
			this->s << ind << "</linearGradient>";
		}
		this->s << std::endl;
	}

	void visit(const RadialGradientElement& e) override{
		auto ind = indentStr(this->indent);

		this->s << ind << "<radialGradient";
		e.attribsToStream(this->s);

		if(e.children.size() == 0){
			this->s << "/>";
		}else{
			this->s << ">" << std::endl;
			this->childrenToStream(e);
			this->s << ind << "</radialGradient>";
		}
		this->s << std::endl;
	}

	void visit(const Gradient::StopElement& e) override{
		auto ind = indentStr(this->indent);
		this->s << ind << "<stop";
		e.attribsToStream(this->s);
		this->s << "/>" << std::endl;
	}

	void visit(const UseElement& e) override{
		auto ind = indentStr(indent);

		s << ind << "<use";
		e.Element::attribsToStream(s);
		e.Transformable::attribsToStream(s);
		e.Styleable::attribsToStream(s);
		e.Rectangle::attribsToStream(s);
		e.Referencing::attribsToStream(s);
		s << "/>";
		s << std::endl;
	}

	void visit(const PathElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<path";

		e.attribsToStream(s);

		s << "/>";
		s << std::endl;
	}

	void visit(const CircleElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<circle";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}
	
	void visit(const PolylineElement& e) override{
		auto ind = indentStr(indent);

		s << ind << "<polyline";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}
	
	void visit(const PolygonElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<polygon";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}
	
	void visit(const EllipseElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<ellipse";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}
	
	void visit(const RectElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<rect";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}

	void visit(const LineElement& e) override{
		auto ind = indentStr(indent);
	
		s << ind << "<line";
		e.attribsToStream(s);
		s << "/>";
		s << std::endl;
	}
};
}

std::string Element::toString() const{
	std::stringstream s;
	
	StreamWriter visitor(s);
	this->accept(visitor);
	
	return s.str();
}
