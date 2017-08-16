#pragma once

#include <string>

namespace svgdom{
struct AspectRatioed{
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
	
	std::string preserveAspectRatioToString()const;
	void parseAndFillPreserveAspectRatio(const std::string& str);
};
}
