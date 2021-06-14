#include "transformable.hpp"

#include <utki/debug.hpp>

#include "../util.hxx"

using namespace svgdom;

std::string transformable::transformations_to_string()const{
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

decltype(transformable::transformations) transformable::parse(std::string_view str){
	decltype(transformable::transformations) ret;

	try{
		utki::string_parser p(str);
		p.skip_whitespaces();

		while(!p.empty()){
			auto transform = p.read_word_until('(');

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

			p.skip_whitespaces();

			if(p.read_char() != '('){
	//			TRACE(<< "error: expected '('" << std::endl)
				return ret; // expected (
			}

			p.skip_whitespaces();

			switch(t.type_){
				default:
					ASSERT(false)
					break;
				case transformation::type::matrix:
					t.a = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					t.b = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					t.c = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					t.d = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					t.e = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					t.f = p.read_number<real>();
					break;
				case transformation::type::translate:
					t.x = p.read_number<real>();					
					p.skip_whitespaces_and_comma();
					try{
						t.y = p.read_number<real>();
					}catch(std::invalid_argument&){
						t.y = 0;
					}
					break;
				case transformation::type::scale:
					t.x = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					try{
						t.y = p.read_number<real>();
					}catch(std::invalid_argument&){
						t.y = t.x;
					}
					break;
				case transformation::type::rotate:
					t.angle = p.read_number<real>();
					p.skip_whitespaces_and_comma();
					try{
						t.x = p.read_number<real>();
					}catch(std::invalid_argument&){
						t.x = 0;
						t.y = 0;
						break;
					}
					
					p.skip_whitespaces_and_comma();
					t.y = p.read_number<real>();
					break;
				case transformation::type::skewy:
				case transformation::type::skewx:
					t.angle = p.read_number<real>();
					break;
			}

			p.skip_whitespaces();

			if(p.read_char() != ')'){
				return ret; // expected )
			}

			ret.push_back(t);

			p.skip_whitespaces_and_comma();
		}
	}catch(std::invalid_argument&
#ifdef DEBUG
			e
#endif
		)
	{
		LOG([&](auto& o){o << "WARNING: transformable::parse(): std::invalid_argument exception caught, ignored." << '\n' << "e.what() = " << e.what();})
		// ignore
	}

	return ret;
}
