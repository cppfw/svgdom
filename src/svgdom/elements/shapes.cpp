#include "shapes.hpp"

#include <sstream>
#include <cctype>

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

// TODO: deprecated, remove.
rectangle rect_element::rectangleDefaultValues = rect_element::rectangle_default_values();

void path_element::accept(visitor& v){
	v.visit(*this);
}

void path_element::accept(const_visitor& v) const{
	v.visit(*this);
}

void rect_element::accept(visitor& v){
	v.visit(*this);
}

void rect_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void circle_element::accept(visitor& v){
	v.visit(*this);
}

void circle_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void ellipse_element::accept(visitor& v){
	v.visit(*this);
}

void ellipse_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void line_element::accept(visitor& v){
	v.visit(*this);
}

void line_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void polygon_element::accept(visitor& v){
	v.visit(*this);
}

void polygon_element::accept(const_visitor& v) const {
	v.visit(*this);
}

void polyline_element::accept(visitor& v){
	v.visit(*this);
}

void polyline_element::accept(const_visitor& v) const {
	v.visit(*this);
}

std::string polyline_shape::points_to_string() const {
	std::stringstream s;
	
	bool isFirst = true;
	for(auto& p : this->points){
		if(isFirst){
			isFirst = false;
		}else{
			s << ',';
		}
		s << p[0] << ',' << p[1];
	}
	return s.str();
}

decltype(path_element::path) path_element::parse(const std::string& str){
	decltype(path_element::path) ret;
	
//	TRACE(<< "str = " << str << std::endl)
	
	std::istringstream s(str);
	s >> std::skipws;
	
	skipWhitespaces(s);
	
	step_type curType = step_type::unknown;
	
	while(!s.eof()){
		ASSERT(!std::isspace(s.peek()))//spaces should be skept
		
//		TRACE(<< "s.peek() = " << char(s.peek()) << std::endl)
		
		{
			auto t = step::char_to_type(s.peek());
			if(t != step_type::unknown){
				curType = t;
				s.get();
			}else if(curType == step_type::unknown){
				curType = step_type::move_abs;
			}else if(curType == step_type::move_abs){
				curType = step_type::line_abs;
			}else if(curType == step_type::move_rel){
				curType = step_type::line_rel;
			}
		}
		
		skipWhitespaces(s);
		
		step cur_step;
		cur_step.type = curType;
		
		switch(cur_step.type){
			case step_type::move_abs:
			case step_type::move_rel:
			case step_type::line_abs:
			case step_type::line_rel:
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "cur_step.x = " << cur_step.x << std::endl)
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "cur_step.y = " << cur_step.y << std::endl)
				break;
			case step_type::close:
				break;
			case step_type::horizontal_line_abs:
			case step_type::horizontal_line_rel:
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::vertical_line_abs:
			case step_type::vertical_line_rel:
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::cubic_abs:
			case step_type::cubic_rel:
				cur_step.x1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::cubic_smooth_abs:
			case step_type::cubic_smooth_rel:
				cur_step.x2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y2 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::quadratic_abs:
			case step_type::quadratic_rel:
				cur_step.x1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y1 = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::quadratic_smooth_abs:
			case step_type::quadratic_smooth_rel:
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step_type::arc_abs:
			case step_type::arc_rel:
				cur_step.rx = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.ry = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x_axis_rotation = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				{
					char f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					cur_step.flags.large_arc = (f != '0');
				}
				skipWhitespacesAndOrComma(s);
				{
					char f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					cur_step.flags.sweep = (f != '0');
				}
				skipWhitespacesAndOrComma(s);
				cur_step.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				cur_step.y = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			default:
				ASSERT(false)
				break;
		}
		
		ret.push_back(cur_step);
		
		skipWhitespacesAndOrComma(s);
	}
	
	return ret;
}

std::string path_element::path_to_string() const {
	std::stringstream s;
	
	step_type curType = step_type::unknown;

	bool first = true;
	
	for(auto& cur_step : this->path){
		if(curType == cur_step.type){
			s << " ";
		}else{
			if (first) {
				first = false;
			} else {
				s << " ";
			}
			
			s << step::type_to_char(cur_step.type);
			curType = cur_step.type;
		}
		
		switch(cur_step.type){
			case step_type::move_abs:
			case step_type::move_rel:
			case step_type::line_abs:
			case step_type::line_rel:
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step_type::close:
				break;
			case step_type::horizontal_line_abs:
			case step_type::horizontal_line_rel:
				s << cur_step.x;
				break;
			case step_type::vertical_line_abs:
			case step_type::vertical_line_rel:
				s << cur_step.y;
				break;
			case step_type::cubic_abs:
			case step_type::cubic_rel:
				s << cur_step.x1;
				s << ",";
				s << cur_step.y1;
				s << " ";
				s << cur_step.x2;
				s << ",";
				s << cur_step.y2;
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step_type::cubic_smooth_abs:
			case step_type::cubic_smooth_rel:
				s << cur_step.x2;
				s << ",";
				s << cur_step.y2;
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step_type::quadratic_abs:
			case step_type::quadratic_rel:
				s << cur_step.x1;
				s << ",";
				s << cur_step.y1;
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step_type::quadratic_smooth_abs:
			case step_type::quadratic_smooth_rel:
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step_type::arc_abs:
			case step_type::arc_rel:
				s << cur_step.rx;
				s << ",";
				s << cur_step.ry;
				s << " ";
				s << cur_step.x_axis_rotation;
				s << " ";
				s << (cur_step.flags.large_arc ? "1" : "0");
				s << ",";
				s << (cur_step.flags.sweep ? "1" : "0");
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			default:
				ASSERT(false)
				break;
		}
	}
	return s.str();
}


char path_element::step::type_to_char(step_type t){
	switch(t){
		case step_type::move_abs:
			return 'M';
		case step_type::move_rel:
			return 'm';
		case step_type::line_abs:
			return 'L';
		case step_type::line_rel:
			return 'l';
		case step_type::close:
			return 'z';
		case step_type::horizontal_line_abs:
			return 'H';
		case step_type::horizontal_line_rel:
			return 'h';
		case step_type::vertical_line_abs:
			return 'V';
		case step_type::vertical_line_rel:
			return 'v';
		case step_type::cubic_abs:
			return 'C';
		case step_type::cubic_rel:
			return 'c';
		case step_type::cubic_smooth_abs:
			return 'S';
		case step_type::cubic_smooth_rel:
			return 's';
		case step_type::quadratic_abs:
			return 'Q';
		case step_type::quadratic_rel:
			return 'q';
		case step_type::quadratic_smooth_abs:
			return 'T';
		case step_type::quadratic_smooth_rel:
			return 't';
		case step_type::arc_abs:
			return 'A';
		case step_type::arc_rel:
			return 'a';
		default:
			ASSERT(false)
			return ' ';
	}
}

path_element::step_type path_element::step::char_to_type(char c){
	switch(c){
		case 'M':
			return step_type::move_abs;
		case 'm':
			return step_type::move_rel;
		case 'z':
		case 'Z':
			return step_type::close;
		case 'L':
			return step_type::line_abs;
		case 'l':
			return step_type::line_rel;
		case 'H':
			return step_type::horizontal_line_abs;
		case 'h':
			return step_type::horizontal_line_rel;
		case 'V':
			return step_type::vertical_line_abs;
		case 'v':
			return step_type::vertical_line_rel;
		case 'C':
			return step_type::cubic_abs;
		case 'c':
			return step_type::cubic_rel;
		case 'S':
			return step_type::cubic_smooth_abs;
		case 's':
			return step_type::cubic_smooth_rel;
		case 'Q':
			return step_type::quadratic_abs;
		case 'q':
			return step_type::quadratic_rel;
		case 'T':
			return step_type::quadratic_smooth_abs;
		case 't':
			return step_type::quadratic_smooth_rel;
		case 'A':
			return step_type::arc_abs;
		case 'a':
			return step_type::arc_rel;
		default:
			return step_type::unknown;
	}
}

decltype(polyline_shape::points) polyline_shape::parse(const std::string& str) {
	decltype(polyline_shape::points) ret;
	
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

