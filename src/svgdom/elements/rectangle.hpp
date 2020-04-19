#pragma once

#include "../length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct rectangle{
	length x = length::make(0, length_unit::unknown);
	length y = length::make(0, length_unit::unknown);
	length width = length::make(100, length_unit::unknown);
	length height = length::make(100, length_unit::unknown);
	
	rectangle(){}
	
	rectangle(length x, length y, length width, length height) :
			x(x), y(y), width(width), height(height)
	{}
	
	bool is_x_specified()const noexcept{
		return this->x.unit != length_unit::unknown;
	}

	// TODO: deprecated, remove.
	bool isXSpecified()const noexcept{
		return this->is_x_specified();
	}
	
	bool is_y_specified()const noexcept{
		return this->y.unit != length_unit::unknown;
	}

	// TODO: deprecated, remove.
	bool isYSpecified()const noexcept{
		return this->is_y_specified();
	}
	
	bool is_width_specified()const noexcept{
		return this->width.unit != length_unit::unknown;
	}

	// TODO: deprecated, remove.
	bool isWidthSpecified()const noexcept{
		return this->is_width_specified();
	}
	
	bool is_height_specified()const noexcept{
		return this->height.unit != length_unit::unknown;
	}

	// TODO: deprecated, remove.
	bool isHeightSpecified()const noexcept{
		return this->is_height_specified();
	}
};

// TODO: deprecated, remove.
typedef rectangle Rectangle;

}
