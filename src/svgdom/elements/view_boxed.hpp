#pragma once

#include <array>
#include <string>

#include "../config.hpp"

namespace svgdom{

/**
 * @brief SVG element which has viewBox attribute.
 * Provides handling of viewBox and preserveAspectRatio attributes.
 */
struct view_boxed{
	std::array<real, 4> view_box{{-1, -1, -1, -1}};

	std::string view_box_to_string()const;

	static decltype(view_box) parse_view_box(const std::string& str);

	bool is_view_box_specified()const{
		return this->view_box[2] >= 0; // width is not negative
	}
};

}
