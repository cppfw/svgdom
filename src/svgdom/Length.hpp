#pragma once

#include <string>

#include "config.hpp"


#ifdef IN
#	undef IN
#endif

namespace svgdom{

/**
 * @brief SVG attribute value of type 'length'.
 */
struct Length{
	enum class Unit_e{
		UNKNOWN,
		NUMBER,
		PERCENT,
		EM,
		EX,
		PX,
		CM,
		IN,
		PT,
		PC,
		MM
	};

	real value;
	Unit_e unit;
	
	static Length parse(const std::string& str);
	
	static Length make(real value, Unit_e unit = Unit_e::NUMBER)noexcept;
	
	bool isValid()const noexcept{
		return this->unit != Unit_e::UNKNOWN;
	}
	
	bool isPercent()const noexcept{
		return this->unit == Unit_e::PERCENT;
	}
	
	real toPx(real dpi)const noexcept;
	
	bool operator!=(const Length& l)const{
		return this->value != l.value || (this->unit != l.unit && this->value != real(0));
	}
};

}

std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
