#include "Transformable.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;


std::string Transformable::transformationsToString() const {
	std::stringstream s;

	bool isFirst = true;

	for(auto& t : this->transformations){
		if(isFirst){
			isFirst = false;
		}else{
			s << " ";
		}

		switch(t.type){
			default:
				ASSERT(false)
				break;
			case Transformation::Type_e::MATRIX:
				s << "matrix(" << t.a << "," << t.b << "," << t.c << "," << t.d << "," << t.e << "," << t.f << ")";
				break;
			case Transformation::Type_e::TRANSLATE:
				s << "translate(" << t.x;
				if(t.y != 0){
					s << "," << t.y;
				}
				s << ")";
				break;
			case Transformation::Type_e::SCALE:
				s << "scale(" << t.x;
				if(t.x != t.y){
					s << "," << t.y;
				}
				s << ")";
				break;
			case Transformation::Type_e::ROTATE:
				s << "rotate(" << t.angle;
				if(t.x != 0 || t.y != 0){
					s << "," << t.x << "," << t.y;
				}
				s << ")";
				break;
			case Transformation::Type_e::SKEWX:
				s << "skewX(" << t.angle << ")";
				break;
			case Transformation::Type_e::SKEWY:
				s << "skewY(" << t.angle << ")";
				break;
		}
	}

	return s.str();
}


decltype(Transformable::transformations) Transformable::parse(const std::string& str){
	std::istringstream s(str);

	s >> std::skipws;

	skipWhitespaces(s);

	decltype(Transformable::transformations) ret;

	while(!s.eof()){
		std::string transform = readTillCharOrWhitespace(s, '(');

//		TRACE(<< "transform = " << transform << std::endl)

		Transformation t;

		if(transform == "matrix"){
			t.type = Transformation::Type_e::MATRIX;
		}else if(transform == "translate"){
			t.type = Transformation::Type_e::TRANSLATE;
		}else if(transform == "scale"){
			t.type = Transformation::Type_e::SCALE;
		}else if(transform == "rotate"){
			t.type = Transformation::Type_e::ROTATE;
		}else if(transform == "skewX"){
			t.type = Transformation::Type_e::SKEWX;
		}else if(transform == "skewY"){
			t.type = Transformation::Type_e::SKEWY;
		}else{
			return ret;//unknown transformation, stop parsing
		}

		skipWhitespaces(s);

		if(s.get() != '('){
//			TRACE(<< "error: expected '('" << std::endl)
			return ret; //expected (
		}

		skipWhitespaces(s);

		switch(t.type){
			default:
				ASSERT(false)
				break;
			case Transformation::Type_e::MATRIX:
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
			case Transformation::Type_e::TRANSLATE:
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
			case Transformation::Type_e::SCALE:
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
			case Transformation::Type_e::ROTATE:
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
			case Transformation::Type_e::SKEWY:
			case Transformation::Type_e::SKEWX:
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
