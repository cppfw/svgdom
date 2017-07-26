#include "Length.hpp"

#include <sstream>
#include <iomanip>

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
