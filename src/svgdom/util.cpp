#include "util.hxx"

#include <sstream>
#include <cctype>

using namespace svgdom;


void svgdom::skipWhitespaces(std::istream& s){
	while(!s.eof()){
		if(!std::isspace(s.peek())){
			break;
		}
		s.get();
	}
}

void svgdom::skipWhitespacesAndOrComma(std::istream& s){
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

void svgdom::skipTillCharInclusive(std::istream& s, char c){
	while(!s.eof()){
		if(s.get() == c){
			break;
		}
	}
}

std::string svgdom::readTillChar(std::istream& s, char c){
	std::stringstream ss;
	while(!s.eof()){
		if(s.peek() == c || s.peek() == std::char_traits<char>::eof()){
			break;
		}
		ss << char(s.get());
	}
	return ss.str();
}

std::string svgdom::readTillCharOrWhitespace(std::istream& s, char c){
	std::stringstream ss;
	while(!s.eof()){
		if(std::isspace(s.peek()) || s.peek() == c || s.peek() == std::char_traits<char>::eof()){
			break;
		}
		ss << char(s.get());
	}
	return ss.str();
}


namespace{
std::string readInNumberString(std::istream& s){
	std::stringstream ss;
	
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
						return ss.str();
					}
					expSign = true;
					if(!exponent){
						return ss.str();
					}
				}
				sign = true;
				break;
			case 'e':
			case 'E':
				sign = true;
				if(exponent){
					return ss.str();
				}
				exponent = true;
				break;
			case '.':
				if(dot || exponent){
					return ss.str();
				}
				sign = true;
				dot = true;
				break;
			default:
				if(!validNumber){
					//WORKAROUND: if no valid number was read then we need to leave stream in failed state
					//to do that, try to read in the float number (we know it should fail since no valid number detected on stream).
					float x;
					s >> x;
				}
				return ss.str();
		}
		ss << char(s.get());
	}
	return ss.str();
}
}


real svgdom::readInReal(std::istream& s) {
	skipWhitespaces(s);
	
	//On MacOS reading in the number which is terminated by non-number and non-whitespace character,
	//e.g. "0c" will result in stream error, i.e. s.fail() will return true and stream will be in unreadable state.
	//To workaround this, need to read in the number to a separate string and parse it from there.
	auto str = readInNumberString(s);
	
	if(str.length() == 0){
		return 0;
	}
	
	std::istringstream iss(str);
	
	//Visual Studio standard library fails to parse numbers to 'float' if it does not actually fit into 'float',
	//for example, parsing of "5.47382e-48" to float fails instead of giving 0.
	//To workaround this read in to the biggest precision type 'long double'.
	long double x;
	iss >> x;
	return real(x);
}

std::string svgdom::trimTail(const std::string& s){
	const auto t = s.find_last_not_of(" \t\n\r");
	if(t == std::string::npos){
		return s;
	}
	
	return s.substr(0, t + 1);
}

std::string svgdom::iriToLocalId(const std::string& iri){
	if(iri.length() != 0 && iri[0] == '#'){
		return iri.substr(1, iri.length() - 1);
	}
	return std::string();
}


CoordinateUnits_e svgdom::parseCoordinateUnits(const std::string& s){
	if(s == "userSpaceOnUse"){
		return CoordinateUnits_e::USER_SPACE_ON_USE;
	}else if(s == "objectBoundingBox"){
		return CoordinateUnits_e::OBJECT_BOUNDING_BOX;
	}
	return CoordinateUnits_e::UNKNOWN;
}


std::string svgdom::coordinateUnitsToString(CoordinateUnits_e u){
	switch(u){
		default:
			return std::string();
		case CoordinateUnits_e::OBJECT_BOUNDING_BOX:
			return "objectBoundingBox";
		case CoordinateUnits_e::USER_SPACE_ON_USE:
			return "userSpaceOnUse";
	}
}


std::array<real, 2> svgdom::parseNumberOptionalNumber(const std::string& s, std::array<real, 2> defaults){
	std::array<real, 2> ret;
	std::istringstream ss(s);
	skipWhitespaces(ss);
	ret[0] = readInReal(ss);
	if(ss.fail()){
		return defaults;
	}
	skipWhitespacesAndOrComma(ss);

	if(ss.eof()){
		ret[1] = defaults[1];
		return ret;
	}
	
	ret[1] = readInReal(ss);
	if(ss.fail()){
		ret[1] = defaults[1];
	}
	return ret;
}

std::string svgdom::numberOptionalNumberToString(std::array<real, 2> non, real optionalNumberDefault){
	std::stringstream ss;
	
	ss << non[0];
	
	if(non[1] != optionalNumberDefault){
		ss << " " << non[1];
	}
	
	return ss.str();
}
