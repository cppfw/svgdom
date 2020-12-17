#include "transformable.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;


std::string transformable::transformations_to_string() const {
	std::stringstream s;

	bool isFirst = true;

	for(auto& t : this->transformations){
		if(isFirst){
			isFirst = false;
		}else{
			s << " ";
		}

		switch(t.type_){
			default:
				ASSERT(false)
				break;
			case transformation::type::matrix:
				s << "matrix(" << t.a << "," << t.b << "," << t.c << "," << t.d << "," << t.e << "," << t.f << ")";
				break;
			case transformation::type::translate:
				s << "translate(" << t.x;
				if(t.y != 0){
					s << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::scale:
				s << "scale(" << t.x;
				if(t.x != t.y){
					s << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::rotate:
				s << "rotate(" << t.angle;
				if(t.x != 0 || t.y != 0){
					s << "," << t.x << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::skewx:
				s << "skewX(" << t.angle << ")";
				break;
			case transformation::type::skewy:
				s << "skewY(" << t.angle << ")";
				break;
		}
	}

	return s.str();
}


decltype(transformable::transformations) transformable::parse(const std::string& str){
	std::istringstream s(str);

	s >> std::skipws;

	skip_whitespaces(s);

	decltype(transformable::transformations) ret;

	while(!s.eof()){
		std::string transform = read_till_char_or_whitespace(s, '(');

//		TRACE(<< "transform = " << transform << std::endl)

		transformation t;

		if(transform == "matrix"){
			t.type_ = transformation::type::matrix;
		}else if(transform == "translate"){
			t.type_ = transformation::type::translate;
		}else if(transform == "scale"){
			t.type_ = transformation::type::scale;
		}else if(transform == "rotate"){
			t.type_ = transformation::type::rotate;
		}else if(transform == "skewX"){
			t.type_ = transformation::type::skewx;
		}else if(transform == "skewY"){
			t.type_ = transformation::type::skewy;
		}else{
			return ret; // unknown transformation, stop parsing
		}

		skip_whitespaces(s);

		if(s.get() != '('){
//			TRACE(<< "error: expected '('" << std::endl)
			return ret; // expected (
		}

		skip_whitespaces(s);

		switch(t.type_){
			default:
				ASSERT(false)
				break;
			case transformation::type::matrix:
				t.a = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.b = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.c = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.d = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.e = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.f = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
			case transformation::type::translate:
				t.x = read_in_real(s);
				if(s.fail()){
//					TRACE(<< "failed to read in x translation" << std::endl)
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.y = read_in_real(s);
				if(s.fail()){
//					TRACE(<< "failed to read in y translation" << std::endl)
					s.clear();
					t.y = 0;
				}
//				TRACE(<< "translation read: x,y = " << t.x << ", " << t.y << std::endl)
				break;
			case transformation::type::scale:
				t.x = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.y = read_in_real(s);
				if(s.fail()){
					s.clear();
					t.y = t.x;
				}
				break;
			case transformation::type::rotate:
				t.angle = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				skip_whitespaces_and_comma(s);
				t.x = read_in_real(s);
				if(s.fail()){
					s.clear();
					t.x = 0;
					t.y = 0;
				}else{
					skip_whitespaces_and_comma(s);
					t.y = read_in_real(s);
					if(s.fail()){
						return ret; // malformed rotate transformation
					}
				}
				break;
			case transformation::type::skewy:
			case transformation::type::skewx:
				t.angle = read_in_real(s);
				if(s.fail()){
					return ret;
				}
				break;
		}

		skip_whitespaces(s);

		if(s.get() != ')'){
			return ret; // expected )
		}

		ret.push_back(t);

		skip_whitespaces_and_comma(s);
	}

	return ret;
}
