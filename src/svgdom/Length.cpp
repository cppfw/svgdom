#include "Length.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>

#include "util.hxx"

using namespace svgdom;


Length Length::make(real value, Unit_e unit) noexcept {
	Length ret;
	
	ret.unit = unit;
	ret.value = value;
	
	return ret;
}


Length Length::parse(const std::string& str) {
	Length ret;

	std::istringstream ss(str);
	
	ss >> std::skipws;
	
	ret.value = readInReal(ss);
	
	std::string u;
	
	ss >> std::setw(2) >> u >> std::setw(0);
	
	if(u.length() == 0){
		ret.unit = Length::Unit_e::NUMBER;
	}else if(u == "%"){
		ret.unit = Length::Unit_e::PERCENT;
	}else if(u == "em"){
		ret.unit = Length::Unit_e::EM;
	}else if(u == "ex"){
		ret.unit = Length::Unit_e::EX;
	}else if(u == "px"){
		ret.unit = Length::Unit_e::PX;
	}else if(u == "cm"){
		ret.unit = Length::Unit_e::CM;
	}else if(u == "mm"){
		ret.unit = Length::Unit_e::MM;
	}else if(u == "in"){
		ret.unit = Length::Unit_e::IN;
	}else if(u == "pt"){
		ret.unit = Length::Unit_e::PT;
	}else if(u == "pc"){
		ret.unit = Length::Unit_e::PC;
	}else{
		ret.unit = Length::Unit_e::UNKNOWN;
	}
	
	return ret;
}

real Length::toPx(real dpi) const noexcept{
	switch(this->unit){
		default:
			return 0;
		case svgdom::Length::Unit_e::NUMBER:
		case svgdom::Length::Unit_e::PX:
			return this->value;
		case svgdom::Length::Unit_e::IN:
			return std::ceil(this->value * dpi);
		case svgdom::Length::Unit_e::CM:
			return std::ceil(this->value * (dpi / real(2.54)));
		case svgdom::Length::Unit_e::MM:
			return std::ceil(this->value * (dpi / real(25.4)));
		case svgdom::Length::Unit_e::PT: // 1pt = 1/72 of an inch
			return std::ceil(this->value * (dpi / real(72)));
		case svgdom::Length::Unit_e::PC: // 1pc = 1/6 of an inch
			return std::ceil(this->value * (dpi / real(6)));
		case svgdom::Length::Unit_e::EM:
		case svgdom::Length::Unit_e::EX:
			//em and ex depend on the font size. Text is not supported by svgdom, so return 0 size.
			return 0;
	}
}


std::ostream& operator<<(std::ostream& s, const Length& l){
	s << l.value;
	
	switch(l.unit){
		case Length::Unit_e::UNKNOWN:
		case Length::Unit_e::NUMBER:
		default:
			break;
		case Length::Unit_e::PERCENT:
			s << "%";
			break;
		case Length::Unit_e::EM:
			s << "em";
			break;
		case Length::Unit_e::EX:
			s << "ex";
			break;
		case Length::Unit_e::PX:
			s << "px";
			break;
		case Length::Unit_e::CM:
			s << "cm";
			break;
		case Length::Unit_e::MM:
			s << "mm";
			break;
		case Length::Unit_e::IN:
			s << "in";
			break;
		case Length::Unit_e::PT:
			s << "pt";
			break;
		case Length::Unit_e::PC:
			s << "pc";
			break;
	}
	
	return s;
}
