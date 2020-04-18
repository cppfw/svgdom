#pragma once

#include <string>

#include "config.hpp"

#ifdef IN
#	undef IN
#endif

namespace svgdom{

enum class length_unit{
	unknown,
	UNKNOWN = unknown, // TODO: deprecated, remove.
	number,
	NUMBER = number, // TODO: deprecated, remove.
	percent,
	PERCENT = percent, // TODO: deprecated, remove.
	em,
	EM = em, // TODO: deprecated, remove.
	ex,
	EX = ex, // TODO: deprecated, remove.
	px,
	PX = px, // TODO: deprecated, remove.
	cm,
	CM = cm, // TODO: deprecated, remove.
	in,
	IN = in, // TODO: deprecated, remove.
	pt,
	PT = pt, // TODO: deprecated, remove.
	pc,
	PC = pc, // TODO: deprecated, remove.
	mm,
	MM = mm // TODO: deprecated, remove.
};

/**
 * @brief SVG attribute value of type 'length'.
 */
struct length{
	// TODO: DEPRECATED, remove.
	typedef length_unit Unit_e;

	real value;
	length_unit unit;
	
	static length parse(const std::string& str);
	
	static length make(real value, length_unit unit = length_unit::number)noexcept;
	
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

std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
