#pragma once

#include <string>

namespace svgdom{
struct aspect_ratioed{
	enum class aspect_ratio_preservation{
		none,
		NONE = none, // TODO: deprecated, remove
		x_min_y_min,
		X_MIN_Y_MIN = x_min_y_min, // TODO: deprecated, remove
		x_mid_y_min,
		X_MID_Y_MIN = x_mid_y_min, // TODO: deprecated, remove
		x_max_y_min,
		X_MAX_Y_MIN = x_max_y_min, // TODO: deprecated, remove
		x_min_y_mid,
		X_MIN_Y_MID = x_min_y_mid, // TODO: deprecated, remove
		x_mid_y_mid,
		X_MID_Y_MID = x_mid_y_mid, // TODO: deprecated, remove
		x_max_y_mid,
		X_MAX_Y_MID = x_max_y_mid, // TODO: deprecated, remove
		x_min_y_max,
		X_MIN_Y_MAX = x_min_y_max, // TODO: deprecated, remove
		x_mid_y_max,
		X_MID_Y_MAX = x_mid_y_max, // TODO: deprecated, remove
		x_max_y_max,
		X_MAX_Y_MAX = x_max_y_max // TODO: deprecated, remove
	};

	// TODO: deprecaed, remove.
	typedef aspect_ratio_preservation PreserveAspectRatio_e;

	struct aspect_ratio_preservation_value{
		aspect_ratio_preservation preserve = aspect_ratio_preservation::none;
		bool defer = false;
		bool slice = false;

		std::string to_string()const;
		void parse(const std::string& str);
	} preserve_aspect_ratio;

	// TODO: deprecated, remove.
	decltype(preserve_aspect_ratio)& preserveAspectRatio = preserve_aspect_ratio;

	// TODO: deprecated, remove.
	std::string preserveAspectRatioToString()const{
		return this->preserve_aspect_ratio.to_string();
	}

	// TODO: deprecated, remove.
	void parseAndFillPreserveAspectRatio(const std::string& str){
		this->preserve_aspect_ratio.parse(str);
	}
};

// TODO: DEPRECATED, remove.
typedef aspect_ratioed AspectRatioed;

}
