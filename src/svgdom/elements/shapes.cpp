#include "shapes.hpp"

#include <sstream>
#include <cctype>

#include <utki/debug.hpp>

#include "../util.hxx"
#include "../visitor.hpp"

using namespace svgdom;

const std::string circle_element::tag = "circle";
const std::string ellipse_element::tag = "ellipse";
const std::string line_element::tag = "line";
const std::string path_element::tag = "path";
const std::string polygon_element::tag = "polygon";
const std::string polyline_element::tag = "polyline";
const std::string rect_element::tag = "rect";

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
	
	skip_whitespaces(s);
	
	step::type curType = step::type::unknown;
	
	while(!s.eof()){
		ASSERT(!std::isspace(s.peek()))//spaces should be skept
		
//		TRACE(<< "s.peek() = " << char(s.peek()) << std::endl)
		
		{
			auto t = step::char_to_type(s.peek());
			if(t != step::type::unknown){
				curType = t;
				s.get();
			}else if(curType == step::type::unknown){
				curType = step::type::move_abs;
			}else if(curType == step::type::move_abs){
				curType = step::type::line_abs;
			}else if(curType == step::type::move_rel){
				curType = step::type::line_rel;
			}
		}
		
		skip_whitespaces(s);
		
		step cur_step;
		cur_step.type_ = curType;
		
		switch(cur_step.type_){
			case step::type::move_abs:
			case step::type::move_rel:
			case step::type::line_abs:
			case step::type::line_rel:
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "cur_step.x = " << cur_step.x << std::endl)
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
//				TRACE(<< "cur_step.y = " << cur_step.y << std::endl)
				break;
			case step::type::close:
				break;
			case step::type::horizontal_line_abs:
			case step::type::horizontal_line_rel:
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::vertical_line_abs:
			case step::type::vertical_line_rel:
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::cubic_abs:
			case step::type::cubic_rel:
				cur_step.x1 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y1 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.x2 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y2 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::cubic_smooth_abs:
			case step::type::cubic_smooth_rel:
				cur_step.x2 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y2 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::quadratic_abs:
			case step::type::quadratic_rel:
				cur_step.x1 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y1 = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::quadratic_smooth_abs:
			case step::type::quadratic_smooth_rel:
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case step::type::arc_abs:
			case step::type::arc_rel:
				cur_step.rx = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.ry = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.x_axis_rotation = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				{
					char f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					cur_step.flags.large_arc = (f != '0');
				}
				skip_whitespaces_and_comma(s);
				{
					char f;
					s >> f;
					if(s.fail()){
						return ret;
					}
					cur_step.flags.sweep = (f != '0');
				}
				skip_whitespaces_and_comma(s);
				cur_step.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				cur_step.y = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			default:
				ASSERT(false)
				break;
		}
		
		ret.push_back(cur_step);
		
		skip_whitespaces_and_comma(s);
	}
	
	return ret;
}

std::string path_element::path_to_string() const {
	std::stringstream s;
	
	step::type curType = step::type::unknown;

	bool first = true;
	
	for(auto& cur_step : this->path){
		if(curType == cur_step.type_){
			s << " ";
		}else{
			if (first) {
				first = false;
			} else {
				s << " ";
			}
			
			s << step::type_to_char(cur_step.type_);
			curType = cur_step.type_;
		}
		
		switch(cur_step.type_){
			case step::type::move_abs:
			case step::type::move_rel:
			case step::type::line_abs:
			case step::type::line_rel:
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step::type::close:
				break;
			case step::type::horizontal_line_abs:
			case step::type::horizontal_line_rel:
				s << cur_step.x;
				break;
			case step::type::vertical_line_abs:
			case step::type::vertical_line_rel:
				s << cur_step.y;
				break;
			case step::type::cubic_abs:
			case step::type::cubic_rel:
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
			case step::type::cubic_smooth_abs:
			case step::type::cubic_smooth_rel:
				s << cur_step.x2;
				s << ",";
				s << cur_step.y2;
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step::type::quadratic_abs:
			case step::type::quadratic_rel:
				s << cur_step.x1;
				s << ",";
				s << cur_step.y1;
				s << " ";
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step::type::quadratic_smooth_abs:
			case step::type::quadratic_smooth_rel:
				s << cur_step.x;
				s << ",";
				s << cur_step.y;
				break;
			case step::type::arc_abs:
			case step::type::arc_rel:
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


char path_element::step::type_to_char(step::type t){
	switch(t){
		case step::type::move_abs:
			return 'M';
		case step::type::move_rel:
			return 'm';
		case step::type::line_abs:
			return 'L';
		case step::type::line_rel:
			return 'l';
		case step::type::close:
			return 'z';
		case step::type::horizontal_line_abs:
			return 'H';
		case step::type::horizontal_line_rel:
			return 'h';
		case step::type::vertical_line_abs:
			return 'V';
		case step::type::vertical_line_rel:
			return 'v';
		case step::type::cubic_abs:
			return 'C';
		case step::type::cubic_rel:
			return 'c';
		case step::type::cubic_smooth_abs:
			return 'S';
		case step::type::cubic_smooth_rel:
			return 's';
		case step::type::quadratic_abs:
			return 'Q';
		case step::type::quadratic_rel:
			return 'q';
		case step::type::quadratic_smooth_abs:
			return 'T';
		case step::type::quadratic_smooth_rel:
			return 't';
		case step::type::arc_abs:
			return 'A';
		case step::type::arc_rel:
			return 'a';
		default:
			ASSERT(false)
			return ' ';
	}
}

path_element::step::type path_element::step::char_to_type(char c){
	switch(c){
		case 'M':
			return step::type::move_abs;
		case 'm':
			return step::type::move_rel;
		case 'z':
		case 'Z':
			return step::type::close;
		case 'L':
			return step::type::line_abs;
		case 'l':
			return step::type::line_rel;
		case 'H':
			return step::type::horizontal_line_abs;
		case 'h':
			return step::type::horizontal_line_rel;
		case 'V':
			return step::type::vertical_line_abs;
		case 'v':
			return step::type::vertical_line_rel;
		case 'C':
			return step::type::cubic_abs;
		case 'c':
			return step::type::cubic_rel;
		case 'S':
			return step::type::cubic_smooth_abs;
		case 's':
			return step::type::cubic_smooth_rel;
		case 'Q':
			return step::type::quadratic_abs;
		case 'q':
			return step::type::quadratic_rel;
		case 'T':
			return step::type::quadratic_smooth_abs;
		case 't':
			return step::type::quadratic_smooth_rel;
		case 'A':
			return step::type::arc_abs;
		case 'a':
			return step::type::arc_rel;
		default:
			return step::type::unknown;
	}
}

decltype(polyline_shape::points) polyline_shape::parse(std::string_view s){
	decltype(polyline_shape::points) ret;
	
	string_parser p(s);

	try{
		while(!s.empty()){
			decltype(ret)::value_type point;

			point[0] = p.read_real();

			p.skip_whitespaces_and_comma();

			point[1] = p.read_real();
			
			ret.push_back(point);
			
			p.skip_whitespaces_and_comma();
		}
	}catch(std::invalid_argument& e){
		// ignore
	}
	
	return ret;
}

