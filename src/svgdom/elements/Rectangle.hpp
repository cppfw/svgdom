#pragma once

#include "../length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct Rectangle{
	Length x = Length::make(0, length_unit::UNKNOWN);
	Length y = Length::make(0, length_unit::UNKNOWN);
	Length width = Length::make(100, length_unit::UNKNOWN);
	Length height = Length::make(100, length_unit::UNKNOWN);
	
	Rectangle(){}
	
	Rectangle(Length x, Length y, Length width, Length height) :
			x(x), y(y), width(width), height(height)
	{}
	
	bool isXSpecified()const noexcept{
		return this->x.unit != length_unit::UNKNOWN;
	}
	
	bool isYSpecified()const noexcept{
		return this->y.unit != length_unit::UNKNOWN;
	}
	
	bool isWidthSpecified()const noexcept{
		return this->width.unit != length_unit::UNKNOWN;
	}
	
	bool isHeightSpecified()const noexcept{
		return this->height.unit != length_unit::UNKNOWN;
	}
};

}
