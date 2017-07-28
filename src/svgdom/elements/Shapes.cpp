#include "Shapes.hpp"

#include <sstream>
#include <cctype>

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../Visitor.hpp"

using namespace svgdom;

void PathElement::accept(Visitor& visitor) const{
	visitor.visit(*this);
}

void RectElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void CircleElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void EllipseElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void LineElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void PolygonElement::accept(Visitor& visistor) const {
	visistor.visit(*this);
}

void PolylineElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void CircleElement::attribsToStream(std::ostream& s) const {
	this->Shape::attribsToStream(s);
	
	if(this->cx.unit != Length::Unit_e::UNKNOWN){
		s << " cx=\"" << this->cx << "\"";
	}
	
	if(this->cy.unit != Length::Unit_e::UNKNOWN){
		s << " cy=\"" << this->cy << "\"";
	}
	
	if(this->r.unit != Length::Unit_e::UNKNOWN){
		s << " r=\"" << this->r << "\"";
	}
}



void EllipseElement::attribsToStream(std::ostream& s) const {
	this->Shape::attribsToStream(s);
	
	if(this->cx.unit != Length::Unit_e::UNKNOWN){
		s << " cx=\"" << this->cx << "\"";
	}
	
	if(this->cy.unit != Length::Unit_e::UNKNOWN){
		s << " cy=\"" << this->cy << "\"";
	}
	
	if(this->rx.unit != Length::Unit_e::UNKNOWN){
		s << " rx=\"" << this->rx << "\"";
	}
	
	if(this->ry.unit != Length::Unit_e::UNKNOWN){
		s << " ry=\"" << this->ry << "\"";
	}
}

void LineElement::attribsToStream(std::ostream& s) const {
	this->Shape::attribsToStream(s);
	
	if(this->x1.unit != Length::Unit_e::UNKNOWN){
		s << " x1=\"" << this->x1 << "\"";
	}
	
	if(this->y1.unit != Length::Unit_e::UNKNOWN){
		s << " y1=\"" << this->y1 << "\"";
	}
	
	if(this->x2.unit != Length::Unit_e::UNKNOWN){
		s << " x2=\"" << this->x2 << "\"";
	}
	
	if(this->y2.unit != Length::Unit_e::UNKNOWN){
		s << " y2=\"" << this->y2 << "\"";
	}
}

void PolylineShape::attribsToStream(std::ostream& s) const {
	this->Shape::attribsToStream(s);
	
	if(this->points.size() != 0){
		s << " points=\"";
		bool isFirst = true;
		for(auto& p : this->points){
			if(isFirst){
				isFirst = false;
			}else{
				s << ',';
			}
			s << p[0] << ',' << p[1];
		}
		s << "\"";
	}
}

void RectElement::attribsToStream(std::ostream& s) const {
	this->Shape::attribsToStream(s);
	this->Rectangle::attribsToStream(s);
	
	if(this->rx.unit != Length::Unit_e::UNKNOWN){
		s << " rx=\"" << this->rx << "\"";
	}
	
	if(this->ry.unit != Length::Unit_e::UNKNOWN){
		s << " ry=\"" << this->ry << "\"";
	}
}

decltype(PathElement::path) PathElement::parse(const std::string& str){
	decltype(PathElement::path) ret;
	
//	TRACE(<< "str = " << str << std::endl)
	
	std::istringstream s(str);
	s >> std::skipws;
	
	skipWhitespaces(s);
	
	Step::Type_e curType = Step::Type_e::UNKNOWN;
	
	while(!s.eof()){
		ASSERT(!std::isspace(s.peek()))//spaces should be skept
		
//		TRACE(<< "s.peek() = " << char(s.peek()) << std::endl)
		
		{
			auto t = Step::charToType(s.peek());
			if(t != Step::Type_e::UNKNOWN){
				curType = t;
				s.get();
			}else if(curType == Step::Type_e::UNKNOWN){
				curType = Step::Type_e::MOVE_ABS;
			}else if(curType == Step::Type_e::MOVE_ABS){
				curType = Step::Type_e::LINE_ABS;
			}else if(curType == Step::Type_e::MOVE_REL){
				curType = Step::Type_e::LINE_REL;
			}
		}
		
		skipWhitespaces(s);
		
		Step step;
		step.type = curType;
		
		switch(step.type){
			case Step::Type_e::MOVE_ABS:
			case Step::Type_e::MOVE_REL:
			case Step::Type_e::LINE_ABS:
			case Step::Type_e::LINE_REL:
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "step.x = " << step.x << std::endl)
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "step.y = " << step.y << std::endl)
				break;
			case Step::Type_e::CLOSE:
				break;
			case Step::Type_e::HORIZONTAL_LINE_ABS:
			case Step::Type_e::HORIZONTAL_LINE_REL:
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::VERTICAL_LINE_ABS:
			case Step::Type_e::VERTICAL_LINE_REL:
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::CUBIC_ABS:
			case Step::Type_e::CUBIC_REL:
				step.x1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.x2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::CUBIC_SMOOTH_ABS:
			case Step::Type_e::CUBIC_SMOOTH_REL:
				step.x2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::QUADRATIC_ABS:
			case Step::Type_e::QUADRATIC_REL:
				step.x1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::QUADRATIC_SMOOTH_ABS:
			case Step::Type_e::QUADRATIC_SMOOTH_REL:
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case Step::Type_e::ARC_ABS:
			case Step::Type_e::ARC_REL:
				step.rx = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.ry = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.xAxisRotation = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				{
					unsigned f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					step.flags.largeArc = (f != 0);
				}
				skipWhitespacesAndOrComma(s);
				{
					unsigned f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					step.flags.sweep = (f != 0);
				}
				skipWhitespacesAndOrComma(s);
				step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			default:
				ASSERT(false)
				break;
		}
		
		ret.push_back(step);
		
		skipWhitespacesAndOrComma(s);
	}
	
	return ret;
}

void Shape::attribsToStream(std::ostream& s) const {
	this->Element::attribsToStream(s);
	this->Transformable::attribsToStream(s);
	this->Styleable::attribsToStream(s);
}


void PathElement::attribsToStream(std::ostream& s) const{
	this->Shape::attribsToStream(s);
	
	if(this->path.size() == 0){
		return;
	}
	
	s << " d=\"";
	
	Step::Type_e curType = Step::Type_e::UNKNOWN;
	
	for(auto& step : this->path){
		if(curType == step.type){
			s << " ";
		}else{
			s << Step::typeToChar(step.type);
			curType = step.type;
		}
		
		switch(step.type){
			case Step::Type_e::MOVE_ABS:
			case Step::Type_e::MOVE_REL:
			case Step::Type_e::LINE_ABS:
			case Step::Type_e::LINE_REL:
				s << step.x;
				s << ",";
				s << step.y;
				break;
			case Step::Type_e::CLOSE:
				break;
			case Step::Type_e::HORIZONTAL_LINE_ABS:
			case Step::Type_e::HORIZONTAL_LINE_REL:
				s << step.x;
				break;
			case Step::Type_e::VERTICAL_LINE_ABS:
			case Step::Type_e::VERTICAL_LINE_REL:
				s << step.y;
				break;
			case Step::Type_e::CUBIC_ABS:
			case Step::Type_e::CUBIC_REL:
				s << step.x1;
				s << ",";
				s << step.y1;
				s << " ";
				s << step.x2;
				s << ",";
				s << step.y2;
				s << " ";
				s << step.x;
				s << ",";
				s << step.y;
				break;
			case Step::Type_e::CUBIC_SMOOTH_ABS:
			case Step::Type_e::CUBIC_SMOOTH_REL:
				s << step.x2;
				s << ",";
				s << step.y2;
				s << " ";
				s << step.x;
				s << ",";
				s << step.y;
				break;
			case Step::Type_e::QUADRATIC_ABS:
			case Step::Type_e::QUADRATIC_REL:
				s << step.x1;
				s << ",";
				s << step.y1;
				s << " ";
				s << step.x;
				s << ",";
				s << step.y;
				break;
			case Step::Type_e::QUADRATIC_SMOOTH_ABS:
			case Step::Type_e::QUADRATIC_SMOOTH_REL:
				s << step.x;
				s << ",";
				s << step.y;
				break;
			case Step::Type_e::ARC_ABS:
			case Step::Type_e::ARC_REL:
				s << step.rx;
				s << ",";
				s << step.ry;
				s << " ";
				s << step.xAxisRotation;
				s << " ";
				s << (step.flags.largeArc ? "1" : "0");
				s << ",";
				s << (step.flags.sweep ? "1" : "0");
				s << " ";
				s << step.x;
				s << ",";
				s << step.y;
				break;
			default:
				ASSERT(false)
				break;
		}
	}
	
	s << "\"";
}

char PathElement::Step::typeToChar(Type_e t){
	switch(t){
		case Step::Type_e::MOVE_ABS:
			return 'M';
		case Step::Type_e::MOVE_REL:
			return 'm';
		case Step::Type_e::LINE_ABS:
			return 'L';
		case Step::Type_e::LINE_REL:
			return 'l';
		case Step::Type_e::CLOSE:
			return 'z';
		case Step::Type_e::HORIZONTAL_LINE_ABS:
			return 'H';
		case Step::Type_e::HORIZONTAL_LINE_REL:
			return 'h';
		case Step::Type_e::VERTICAL_LINE_ABS:
			return 'V';
		case Step::Type_e::VERTICAL_LINE_REL:
			return 'v';
		case Step::Type_e::CUBIC_ABS:
			return 'C';
		case Step::Type_e::CUBIC_REL:
			return 'c';
		case Step::Type_e::CUBIC_SMOOTH_ABS:
			return 'S';
		case Step::Type_e::CUBIC_SMOOTH_REL:
			return 's';
		case Step::Type_e::QUADRATIC_ABS:
			return 'Q';
		case Step::Type_e::QUADRATIC_REL:
			return 'q';
		case Step::Type_e::QUADRATIC_SMOOTH_ABS:
			return 'T';
		case Step::Type_e::QUADRATIC_SMOOTH_REL:
			return 't';
		case Step::Type_e::ARC_ABS:
			return 'A';
		case Step::Type_e::ARC_REL:
			return 'a';
		default:
			ASSERT(false)
			return ' ';
	}
}


PathElement::Step::Type_e PathElement::Step::charToType(char c){
	switch(c){
		case 'M':
			return Step::Type_e::MOVE_ABS;
		case 'm':
			return Step::Type_e::MOVE_REL;
		case 'z':
		case 'Z':
			return Step::Type_e::CLOSE;
		case 'L':
			return Step::Type_e::LINE_ABS;
		case 'l':
			return Step::Type_e::LINE_REL;
		case 'H':
			return Step::Type_e::HORIZONTAL_LINE_ABS;
		case 'h':
			return Step::Type_e::HORIZONTAL_LINE_REL;
		case 'V':
			return Step::Type_e::VERTICAL_LINE_ABS;
		case 'v':
			return Step::Type_e::VERTICAL_LINE_REL;
		case 'C':
			return Step::Type_e::CUBIC_ABS;
		case 'c':
			return Step::Type_e::CUBIC_REL;
		case 'S':
			return Step::Type_e::CUBIC_SMOOTH_ABS;
		case 's':
			return Step::Type_e::CUBIC_SMOOTH_REL;
		case 'Q':
			return Step::Type_e::QUADRATIC_ABS;
		case 'q':
			return Step::Type_e::QUADRATIC_REL;
		case 'T':
			return Step::Type_e::QUADRATIC_SMOOTH_ABS;
		case 't':
			return Step::Type_e::QUADRATIC_SMOOTH_REL;
		case 'A':
			return Step::Type_e::ARC_ABS;
		case 'a':
			return Step::Type_e::ARC_REL;
		default:
			return Step::Type_e::UNKNOWN;
	}
}

decltype(PolylineShape::points) PolylineShape::parse(const std::string& str) {
	decltype(PolylineShape::points) ret;
	
	std::istringstream s(str);
	s >> std::skipws;
	
	skipWhitespaces(s);
	
	while(!s.eof()){
		decltype(ret)::value_type p;
		p[0] = readInReal(s);
		skipWhitespacesAndOrComma(s);
		p[1] = readInReal(s);
		
		if(s.fail()){
			break;
		}
		
		ret.push_back(p);
		
		skipWhitespacesAndOrComma(s);
	}
	
	return ret;
}

