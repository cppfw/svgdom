/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

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

decltype(path_element::path) path_element::parse(std::string_view str){
	decltype(path_element::path) ret;
	
	try{
		utki::string_parser p(str);
		
		p.skip_whitespaces();
		
		step::type cur_step_type = step::type::unknown;
		
		while(!p.empty()){
			ASSERT(!utki::string_parser::is_space(p.peek_char())) // spaces should be skept
			
			{
				auto t = step::char_to_type(p.peek_char());
				if(t != step::type::unknown){
					cur_step_type = t;
					p.read_char();
				}else if(cur_step_type == step::type::unknown){
					cur_step_type = step::type::move_abs;
				}else if(cur_step_type == step::type::move_abs){
					cur_step_type = step::type::line_abs;
				}else if(cur_step_type == step::type::move_rel){
					cur_step_type = step::type::line_rel;
				}
			}
			
			p.skip_whitespaces();
			
			step cur_step;
			cur_step.type_ = cur_step_type;
			
			switch(cur_step.type_){
				case step::type::move_abs:
				case step::type::move_rel:
				case step::type::line_abs:
				case step::type::line_rel:
					cur_step.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				case step::type::close:
					break;
				case step::type::horizontal_line_abs:
				case step::type::horizontal_line_rel:
					cur_step.x = p.read_number<real>();
					break;
				case step::type::vertical_line_abs:
				case step::type::vertical_line_rel:
					cur_step.y = p.read_number<real>();
					break;
				case step::type::cubic_abs:
				case step::type::cubic_rel:
					cur_step.x1 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y1 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.x2 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y2 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				case step::type::cubic_smooth_abs:
				case step::type::cubic_smooth_rel:
					cur_step.x2 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y2 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				case step::type::quadratic_abs:
				case step::type::quadratic_rel:
					cur_step.x1 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y1 = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.x = p.read_number<real>();					
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				case step::type::quadratic_smooth_abs:
				case step::type::quadratic_smooth_rel:
					cur_step.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				case step::type::arc_abs:
				case step::type::arc_rel:
					cur_step.rx = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.ry = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.x_axis_rotation = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.flags.large_arc = (p.read_char() != '0');
					p.skip_whitespaces_and_comma();
					cur_step.flags.sweep = (p.read_char() != '0');
					p.skip_whitespaces_and_comma();
					cur_step.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					cur_step.y = p.read_number<real>();
					break;
				default:
					ASSERT(false)
					break;
			}
			
			ret.push_back(cur_step);
			
			p.skip_whitespaces_and_comma();
		}
	}catch(std::invalid_argument&
#ifdef DEBUG
			e
#endif
		)
	{
		LOG([&](auto& o){o << "WARNING: path_element::parse(): std::invalid_argument exception caught, ignored." << '\n' << "e.what() = " << e.what();})
		// ignore
	}
	
	return ret;
}

std::string path_element::path_to_string() const {
	std::stringstream s;
	
	step::type cur_step_type = step::type::unknown;

	bool first = true;
	
	for(auto& cur_step : this->path){
		if(cur_step_type == cur_step.type_){
			s << " ";
		}else{
			if (first) {
				first = false;
			} else {
				s << " ";
			}
			
			s << step::type_to_char(cur_step.type_);
			cur_step_type = cur_step.type_;
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
	
	utki::string_parser p(s);

	try{
		while(!s.empty()){
			decltype(ret)::value_type point;

			point[0] = p.read_number<real>();

			p.skip_whitespaces_and_comma();

			point[1] = p.read_number<real>();
			
			ret.push_back(point);
			
			p.skip_whitespaces_and_comma();
		}
	}catch(std::invalid_argument&
#ifdef DEBUG
			e
#endif
		)
	{
		LOG([&](auto& o){o << "WARNING: polyline_shape::parse(): std::invalid_argument exception caught, ignored." << '\n' << "e.what() = " << e.what();})
		// ignore
	}
	
	return ret;
}

