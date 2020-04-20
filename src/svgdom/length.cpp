#include "length.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>

#include "util.hxx"

using namespace svgdom;

length length::parse(const std::string& str) {
	length ret;

	std::istringstream ss(str);
	
	ss >> std::skipws;
	
	ret.value = readInReal(ss);
	
	std::string u;
	
	ss >> std::setw(2) >> u >> std::setw(0);
	
	if(u.length() == 0){
		ret.unit = length::Unit_e::NUMBER;
	}else if(u == "%"){
		ret.unit = length::Unit_e::PERCENT;
	}else if(u == "em"){
		ret.unit = length::Unit_e::EM;
	}else if(u == "ex"){
		ret.unit = length::Unit_e::EX;
	}else if(u == "px"){
		ret.unit = length::Unit_e::PX;
	}else if(u == "cm"){
		ret.unit = length::Unit_e::CM;
	}else if(u == "mm"){
		ret.unit = length::Unit_e::MM;
	}else if(u == "in"){
		ret.unit = length::Unit_e::IN;
	}else if(u == "pt"){
		ret.unit = length::Unit_e::PT;
	}else if(u == "pc"){
		ret.unit = length::Unit_e::PC;
	}else{
		ret.unit = length::Unit_e::UNKNOWN;
	}
	
	return ret;
}

real length::to_px(real dpi) const noexcept{
	switch(this->unit){
		default:
			return 0;
		case svgdom::length::Unit_e::NUMBER:
		case svgdom::length::Unit_e::PX:
			return this->value;
		case svgdom::length::Unit_e::IN:
			return std::ceil(this->value * dpi);
		case svgdom::length::Unit_e::CM:
			return std::ceil(this->value * (dpi / real(2.54)));
		case svgdom::length::Unit_e::MM:
			return std::ceil(this->value * (dpi / real(25.4)));
		case svgdom::length::Unit_e::PT: // 1 pt = 1/72 of an inch
			return std::ceil(this->value * (dpi / real(72)));
		case svgdom::length::Unit_e::PC: // 1 pc = 1/6 of an inch
			return std::ceil(this->value * (dpi / real(6)));
		case svgdom::length::Unit_e::EM:
		case svgdom::length::Unit_e::EX:
			// em and ex depend on the font size. Text is not supported by svgdom, so return 0 size.
			return 0;
	}
}

std::ostream& operator<<(std::ostream& s, const length& l){
	s << l.value;
	
	switch(l.unit){
		case length::Unit_e::UNKNOWN:
		case length::Unit_e::NUMBER:
		default:
			break;
		case length::Unit_e::PERCENT:
			s << "%";
			break;
		case length::Unit_e::EM:
			s << "em";
			break;
		case length::Unit_e::EX:
			s << "ex";
			break;
		case length::Unit_e::PX:
			s << "px";
			break;
		case length::Unit_e::CM:
			s << "cm";
			break;
		case length::Unit_e::MM:
			s << "mm";
			break;
		case length::Unit_e::IN:
			s << "in";
			break;
		case length::Unit_e::PT:
			s << "pt";
			break;
		case length::Unit_e::PC:
			s << "pc";
			break;
	}
	
	return s;
}
