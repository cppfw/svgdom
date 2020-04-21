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
	std::array<real, 4> view_box = {{-1, -1, -1, -1}};

	// TODO: deprecated, remove.
	decltype(view_box)& viewBox = view_box;

	std::string view_box_to_string()const;

	// TODO: deprecated, remove.
	std::string viewBoxToString()const{
		return this->view_box_to_string();
	}

	static decltype(view_box) parse_view_box(const std::string& str);

	// TODO: deprecated, remove.
	static decltype(view_box) parseViewbox(const std::string& str){
		return parse_view_box(str);
	}

	bool is_view_box_specified()const{
		return this->view_box[2] >= 0; // width is not negative
	}

	// TODO: deprecated, remove.
	bool isViewBoxSpecified()const{
		return this->is_view_box_specified();
	}
};

// TODO: deprecated, remove.
typedef view_boxed ViewBoxed;

}
