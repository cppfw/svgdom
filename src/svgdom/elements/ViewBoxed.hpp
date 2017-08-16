#pragma once

#include <array>

#include "../config.hpp"

namespace svgdom{

/**
 * @brief SVG element which has viewBox attribute.
 * Provides handling of viewBox and preserveAspectRatio attributes.
 */
struct ViewBoxed {
	std::array<real, 4> viewBox = {{-1, -1, -1, -1}};

	std::string viewBoxToString()const;

	static decltype(viewBox) parseViewbox(const std::string& str);

	bool isViewBoxSpecified()const {
		return this->viewBox[2] >= 0;//width is not negative
	}
};

}
