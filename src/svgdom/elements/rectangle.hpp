#pragma once

#include "../length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct rectangle{
	length x = length(0, length_unit::unknown);
	length y = length(0, length_unit::unknown);
	length width = length(100, length_unit::unknown);
	length height = length(100, length_unit::unknown);
	
	rectangle(){}
	
	constexpr rectangle(length x, length y, length width, length height) :
			x(x), y(y), width(width), height(height)
	{}
	
	bool is_x_specified()const noexcept{
		return this->x.unit != length_unit::unknown;
	}

	bool is_y_specified()const noexcept{
		return this->y.unit != length_unit::unknown;
	}
	
	bool is_width_specified()const noexcept{
		return this->width.unit != length_unit::unknown;
	}
	
	bool is_height_specified()const noexcept{
		return this->height.unit != length_unit::unknown;
	}
};

}
