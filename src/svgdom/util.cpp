#include "util.hxx"

#include <utki/string.hpp>

#include <sstream>
#include <cctype>
#include <vector>
#include <charconv>

using namespace svgdom;

void string_parser::skip_whitespaces(){
	size_t pos = 0;
	for(char c : this->view){
		if(!std::isspace(c)){
			break;
		}
		++pos;
	}
	this->view = this->view.substr(pos);
}

void string_parser::skip_whitespaces_and_comma(){
	size_t pos = 0;

	bool comma_skipped = false;
	for(char c : this->view){
		if(std::isspace(c)){
			++pos;
		}else if(c == ','){
			if(comma_skipped){
				break;
			}
			++pos;
			comma_skipped = true;
		}else{
			break;
		}
	}
	this->view = this->view.substr(pos);
}

std::string_view string_parser::read_word(){
	for(auto i = this->view.begin(); i != this->view.end(); ++i){
		if(std::isspace(*i)){
			auto dist = std::distance(this->view.begin(), i);
			auto ret = this->view.substr(0, dist);
			this->view = this->view.substr(dist);
			return ret;
		}
	}

	auto ret = this->view;

	this->view = std::string_view();

	return ret;
}

real string_parser::read_real(){
	real ret;

	char* end;

	if constexpr (std::is_same<real, float>::value){
		ret = real(std::strtof(this->view.data(), &end));
	}else if constexpr (std::is_same<real, double>::value){
		ret = real(std::strtod(this->view.data(), &end));
	}else{
		ret = real(std::strtold(this->view.data(), &end));
	}

	if(end == this->view.data()){
		throw std::invalid_argument("string_parser::read_real(): could not parse real number");
	}

	this->view = std::string_view(end, this->view.data() + this->view.size() - end);

	return ret;
}

void svgdom::skip_whitespaces(std::istream& s){
	while(!s.eof()){
		if(!std::isspace(s.peek())){
			break;
		}
		s.get();
	}
}

void svgdom::skip_whitespaces_and_comma(std::istream& s){
	bool commaSkipped = false;
	while(!s.eof()){
		if(std::isspace(s.peek())){
			s.get();
		}else if(s.peek() == ','){
			if(commaSkipped){
				break;
			}
			s.get();
			commaSkipped = true;
		}else{
			break;
		}
	}
}

void svgdom::skip_till_char_inclusive(std::istream& s, char c){
	while(!s.eof()){
		if(s.get() == c){
			break;
		}
	}
}

std::string svgdom::read_till_char(std::istream& s, char c){
	std::vector<char> ss;
	while(!s.eof()){
		if(s.peek() == c || s.peek() == std::char_traits<char>::eof()){
			break;
		}
		ss.push_back(char(s.get()));
	}
	return utki::make_string(ss);
}

std::string svgdom::read_till_char_or_whitespace(std::istream& s, char c){
	std::vector<char> ss;
	while(!s.eof()){
		if(std::isspace(s.peek()) || s.peek() == c || s.peek() == std::char_traits<char>::eof()){
			break;
		}
		ss.push_back(char(s.get()));
	}
	return utki::make_string(ss);
}

namespace{
std::string read_in_number_string(std::istream& s){
	std::vector<char> ss;
	
	bool sign = false;
	bool expSign = false;
	bool exponent = false;
	bool dot = false;
	bool validNumber = false;
	
	while(!s.eof()){
		auto c = char(s.peek());
//		TRACE(<< "c = " << char(c) << std::endl)
		switch(c){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				sign = true;
				if(exponent){
					expSign = true;
				}
				validNumber = true;
				break;
			case '+':
			case '-':
				if(sign){
					if(expSign){
						return utki::make_string(ss);
					}
					expSign = true;
					if(!exponent){
						return utki::make_string(ss);
					}
				}
				sign = true;
				break;
			case 'e':
			case 'E':
				sign = true;
				if(exponent){
					return utki::make_string(ss);
				}
				exponent = true;
				break;
			case '.':
				if(dot || exponent){
					return utki::make_string(ss);
				}
				sign = true;
				dot = true;
				break;
			default:
				if(!validNumber){
					// WORKAROUND: if no valid number was read then we need to leave stream in failed state
					// to do that, try to read in the float number (we know it should fail since no valid number detected on stream).
					float x;
					s >> x;
				}
				return utki::make_string(ss);
		}
		ss.push_back(char(s.get()));
	}
	return utki::make_string(ss);
}
}

real svgdom::read_in_real(std::istream& s){
	skip_whitespaces(s);
	
	// On MacOS reading in the number which is terminated by non-number and non-whitespace character,
	// e.g. "0c" will result in stream error, i.e. s.fail() will return true and stream will be in unreadable state.
	// To workaround this, need to read in the number to a separate string and parse it from there.
	auto str = read_in_number_string(s);
	
	try{
		return string_parser(str).read_real();
	}catch(...){
		return 0;
	}
}

std::string svgdom::trim_tail(const std::string& s){
	const auto t = s.find_last_not_of(" \t\n\r");
	if(t == std::string::npos){
		return s;
	}
	
	return s.substr(0, t + 1);
}

std::string svgdom::iri_to_local_id(const std::string& iri){
	if(iri.length() != 0 && iri[0] == '#'){
		return iri.substr(1, iri.length() - 1);
	}
	return std::string();
}

coordinate_units svgdom::parse_coordinate_units(const std::string& s){
	if(s == "userSpaceOnUse"){
		return coordinate_units::user_space_on_use;
	}else if(s == "objectBoundingBox"){
		return coordinate_units::object_bounding_box;
	}
	return coordinate_units::unknown;
}

std::string svgdom::coordinate_units_to_string(coordinate_units u){
	switch(u){
		default:
			return std::string();
		case coordinate_units::object_bounding_box:
			return "objectBoundingBox";
		case coordinate_units::user_space_on_use:
			return "userSpaceOnUse";
	}
}

r4::vector2<real> svgdom::parse_number_and_optional_number(std::string_view s, r4::vector2<real> defaults){
	r4::vector2<real> ret;

	string_parser p(s);

	try{
		ret[0] = p.read_real();
	}catch(std::invalid_argument& e){
		return defaults;
	}

	p.skip_whitespaces_and_comma();

	try{
		ret[1] = p.read_real();
	}catch(std::invalid_argument& e){
		ret[1] = defaults[1];
	}

	return ret;
}

std::string svgdom::number_and_optional_number_to_string(std::array<real, 2> non, real optionalNumberDefault){
	std::stringstream ss;
	
	ss << non[0];
	
	if(non[1] != optionalNumberDefault){
		ss << " " << non[1];
	}
	
	return ss.str();
}
