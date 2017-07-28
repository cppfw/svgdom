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

	enum class PreserveAspectRatio_e{
		NONE,
		X_MIN_Y_MIN,
		X_MID_Y_MIN,
		X_MAX_Y_MIN,
		X_MIN_Y_MID,
		X_MID_Y_MID,
		X_MAX_Y_MID,
		X_MIN_Y_MAX,
		X_MID_Y_MAX,
		X_MAX_Y_MAX
	};

	struct {
		PreserveAspectRatio_e preserve = PreserveAspectRatio_e::NONE;
		bool defer = false;
		bool slice = false;
	} preserveAspectRatio;

	void parseAndFillPreserveAspectRatio(const std::string& str);

	static decltype(viewBox) parseViewbox(const std::string& str);

	void attribsToStream(std::ostream& s)const;

	bool isViewBoxSpecified()const {
		return this->viewBox[2] >= 0;//width is not negative
	}
};

}
