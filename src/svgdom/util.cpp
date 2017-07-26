#include "util.hxx"

#include <sstream>

using namespace svgdom;


void svgdom::skipWhitespaces(std::istream& s){
	while(!s.eof()){
		if(!std::isspace(s.peek())){
			break;
		}
		s.get();
	}
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
	std::istringstream iss(readInNumberString(s));
	
	//Visual Studio standard library fails to parse numbers to 'float' if it does not actually fit into 'float',
	//for example, parsing of "5.47382e-48" to float fails instead of giving 0.
	//To workaround this read in to the biggest precision type 'long double'.
	long double x;
	iss >> x;
	return real(x);
}

