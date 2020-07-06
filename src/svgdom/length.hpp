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
	
	static length parse(const std::string& str);
	
	length() = default;

	constexpr length(real value, length_unit unit = length_unit::number) :
			value(value),
			unit(unit)
	{}

	//TODO: deprecated, remove.
	static constexpr length make(real value, length_unit unit = length_unit::number)noexcept{
		return length(value, unit);
	}
	
	bool is_valid()const noexcept{
		return this->unit != length_unit::unknown;
	}

	// TODO: deprecated, remove.
	bool isValid()const noexcept{
		return this->is_valid();
	}
	
	bool is_percent()const noexcept{
		return this->unit == length_unit::percent;
	}

	// TODO: deprecated, remove.
	bool isPercent()const noexcept{
		return this->is_percent();
	}
	
	real to_px(real dpi)const noexcept;

	// TODO: deprecated, remove.
	real toPx(real dpi)const noexcept{
		return this->to_px(dpi);
	}
	
	bool operator!=(const length& l)const{
		return this->value != l.value || (this->unit != l.unit && this->value != real(0));
	}
};

// TODO: DEPRECATED, remove.
typedef length Length;

}

std::ostream& operator<<(std::ostream& s, const svgdom::length& l);
