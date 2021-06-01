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

void string_parser::skip_inclusive_until(char c){
	for(; !this->view.empty(); this->view = this->view.substr(1)){
		if(this->view.front() == c){
			this->view = this->view.substr(1);
			break;
		}
	}
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

std::string_view string_parser::read_word_until(char until_char){
	for(auto i = this->view.begin(); i != this->view.end(); ++i){
		if(std::isspace(*i) || *i == until_char){
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

void string_parser::throw_if_empty(){
	if(this->view.empty()){
		throw std::invalid_argument("string_parser string is empty");
	}
}

char string_parser::read_char(){
	this->throw_if_empty();

	char ret = this->view.front();

	this->view = this->view.substr(1);

	return ret;
}

char string_parser::peek_char(){
	this->throw_if_empty();

	return this->view.front();
}

std::string_view string_parser::read_chars(size_t n){
	using std::min;
	n = min(n, this->view.size());
	auto ret = std::string_view(this->view.data(), n);

	this->view = this->view.substr(n);

	return ret;
}

std::string_view string_parser::read_chars_until(char until_char){
	for(auto i = this->view.begin(); i != this->view.end(); ++i){
		if(*i == until_char){
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

std::string_view svgdom::trim_tail(std::string_view s){
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
		ret[0] = p.read_real<real>();
	}catch(std::invalid_argument& e){
		return defaults;
	}

	p.skip_whitespaces_and_comma();

	try{
		ret[1] = p.read_real<real>();
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
