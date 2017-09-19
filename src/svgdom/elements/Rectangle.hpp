#pragma once

#include "../Length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct Rectangle{
	Length x = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y = Length::make(0, Length::Unit_e::UNKNOWN);
	Length width = Length::make(100, Length::Unit_e::UNKNOWN);
	Length height = Length::make(100, Length::Unit_e::UNKNOWN);
	
	Rectangle(){}
	
	Rectangle(Length x, Length y, Length width, Length height) :
			x(x), y(y), width(width), height(height)
	{}
	
	bool isXSpecified()const noexcept{
		return this->x.unit != Length::Unit_e::UNKNOWN;
	}
	
	bool isYSpecified()const noexcept{
		return this->y.unit != Length::Unit_e::UNKNOWN;
	}
	
	bool isWidthSpecified()const noexcept{
		return this->width.unit != Length::Unit_e::UNKNOWN;
	}
	
	bool isHeightSpecified()const noexcept{
		return this->height.unit != Length::Unit_e::UNKNOWN;
	}
};

}
