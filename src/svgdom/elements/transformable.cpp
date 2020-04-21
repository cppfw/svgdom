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
			case transformation::type::MATRIX:
				s << "matrix(" << t.a << "," << t.b << "," << t.c << "," << t.d << "," << t.e << "," << t.f << ")";
				break;
			case transformation::type::TRANSLATE:
				s << "translate(" << t.x;
				if(t.y != 0){
					s << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::SCALE:
				s << "scale(" << t.x;
				if(t.x != t.y){
					s << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::ROTATE:
				s << "rotate(" << t.angle;
				if(t.x != 0 || t.y != 0){
					s << "," << t.x << "," << t.y;
				}
				s << ")";
				break;
			case transformation::type::SKEWX:
				s << "skewX(" << t.angle << ")";
				break;
			case transformation::type::SKEWY:
				s << "skewY(" << t.angle << ")";
				break;
		}
	}

	return s.str();
}


decltype(transformable::transformations) transformable::parse(const std::string& str){
	std::istringstream s(str);

	s >> std::skipws;

	skipWhitespaces(s);

	decltype(transformable::transformations) ret;

	while(!s.eof()){
		std::string transform = readTillCharOrWhitespace(s, '(');

//		TRACE(<< "transform = " << transform << std::endl)

		transformation t;

		if(transform == "matrix"){
			t.type_ = transformation::type::MATRIX;
		}else if(transform == "translate"){
			t.type_ = transformation::type::TRANSLATE;
		}else if(transform == "scale"){
			t.type_ = transformation::type::SCALE;
		}else if(transform == "rotate"){
			t.type_ = transformation::type::ROTATE;
		}else if(transform == "skewX"){
			t.type_ = transformation::type::SKEWX;
		}else if(transform == "skewY"){
			t.type_ = transformation::type::SKEWY;
		}else{
			return ret; // unknown transformation, stop parsing
		}

		skipWhitespaces(s);

		if(s.get() != '('){
//			TRACE(<< "error: expected '('" << std::endl)
			return ret; // expected (
		}

		skipWhitespaces(s);

		switch(t.type_){
			default:
				ASSERT(false)
				break;
			case transformation::type::MATRIX:
				t.a = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.b = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.c = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.d = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.e = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.f = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
			case transformation::type::TRANSLATE:
				t.x = readInReal(s);
				if(s.fail()){
//					TRACE(<< "failed to read in x translation" << std::endl)
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.y = readInReal(s);
				if(s.fail()){
//					TRACE(<< "failed to read in y translation" << std::endl)
					s.clear();
					t.y = 0;
				}
//				TRACE(<< "translation read: x,y = " << t.x << ", " << t.y << std::endl)
				break;
			case transformation::type::SCALE:
				t.x = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.y = readInReal(s);
				if(s.fail()){
					s.clear();
					t.y = t.x;
				}
				break;
			case transformation::type::ROTATE:
				t.angle = readInReal(s);
				if(s.fail()){
					return ret;
				}
				skipWhitespacesAndOrComma(s);
				t.x = readInReal(s);
				if(s.fail()){
					s.clear();
					t.x = 0;
					t.y = 0;
				}else{
					skipWhitespacesAndOrComma(s);
					t.y = readInReal(s);
					if(s.fail()){
						return ret;//malformed rotate transformation
					}
				}
				break;
			case transformation::type::SKEWY:
			case transformation::type::SKEWX:
				t.angle = readInReal(s);
				if(s.fail()){
					return ret;
				}
				break;
		}

		skipWhitespaces(s);

		if(s.get() != ')'){
			return ret;//expected )
		}

		ret.push_back(t);

		skipWhitespacesAndOrComma(s);
	}

	return ret;
}
