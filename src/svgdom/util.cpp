#include "util.hxx"

#include <utki/string.hpp>

#include <sstream>
#include <cctype>
#include <vector>
#include <charconv>

using namespace svgdom;

void svgdom::skip_whitespaces(std::istream& s){
	while(!s.eof()){
		if(!std::isspace(s.peek())){
			break;
		}
		s.get();
	}
}

std::string_view svgdom::skip_whitespaces(std::string_view s){
	size_t pos = 0;
	for(char c : s){
		if(!std::isspace(c)){
			break;
		}
		++pos;
	}
	return s.substr(pos);
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

std::string_view svgdom::skip_whitespaces_and_comma(std::string_view str){
	size_t pos = 0;

	bool comma_skipped = false;
	for(char c : str){
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
	return str.substr(pos);
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

parse_real_result svgdom::parse_real(std::string_view str){
	if(str.empty()){
		return {0, 0};
	}
	
	parse_real_result ret;

	char* end;

	if constexpr (std::is_same<real, float>::value){
		ret.number = real(std::strtof(str.data(), &end));
	}else if constexpr (std::is_same<real, double>::value){
		ret.number = real(std::strtod(str.data(), &end));
	}else{
		ret.number = real(std::strtold(str.data(), &end));
	}

	ret.view = std::string_view(end, str.data() + str.size() - end);

	if(end == str.data()){
		ret.error = true;
	}

	return ret;
}

read_word_result svgdom::read_word(std::string_view str){
	read_word_result ret;

	for(auto i = str.begin(); i != str.end(); ++i){
		if(std::isspace(*i)){
			auto dist = std::distance(str.begin(), i);
			ret.word = str.substr(0, dist);
			ret.view = str.substr(dist);
			return ret;
		}
	}

	ret.word = str;

	return ret;
}

real svgdom::read_in_real(std::istream& s){
	skip_whitespaces(s);
	
	// On MacOS reading in the number which is terminated by non-number and non-whitespace character,
	// e.g. "0c" will result in stream error, i.e. s.fail() will return true and stream will be in unreadable state.
	// To workaround this, need to read in the number to a separate string and parse it from there.
	auto str = read_in_number_string(s);
	
	return parse_real(str).number;
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

	{
		auto r = parse_real(s);
		if(r.error){
			return defaults;
		}

		ret[0] = r.number;

		s = r.view;
	}

	s = skip_whitespaces_and_comma(s);

	if(s.empty()){
		ret[1] = defaults[1];
		return ret;
	}

	{
		auto r = parse_real(s);
		if(r.error){
			ret[1] = defaults[1];
		}else{
			ret[1] = r.number;
		}
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
