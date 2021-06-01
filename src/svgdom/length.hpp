#pragma once

#include <string>

#include "config.hpp"

#ifdef IN
#	undef IN
#endif

namespace svgdom{

enum class length_unit{
	unknown,
	number,
	percent,
	em,
	ex,
	px,
	cm,
	in,
	pt,
	pc,
	mm
};

/**
 * @brief SVG attribute value of type 'length'.
 */
struct length{
	real value;
	length_unit unit;
	
	static length parse(std::string_view str);
	
	length() = default;

	constexpr length(real value, length_unit unit = length_unit::number) :
			value(value),
			unit(unit)
	{}

	bool is_valid()const noexcept{
		return this->unit != length_unit::unknown;
	}

	bool is_percent()const noexcept{
		return this->unit == length_unit::percent;
	}
	
	real to_px(real dpi)const noexcept;
	
	bool operator!=(const length& l)const{
		return this->value != l.value || (this->unit != l.unit && this->value != real(0));
	}
};

}

std::ostream& operator<<(std::ostream& s, const svgdom::length& l);
