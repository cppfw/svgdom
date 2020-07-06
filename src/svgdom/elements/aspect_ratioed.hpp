#pragma once

#include <string>

namespace svgdom{
struct aspect_ratioed{
	enum class aspect_ratio_preservation{
		none,
		x_min_y_min,
		x_mid_y_min,
		x_max_y_min,
		x_min_y_mid,
		x_mid_y_mid,
		x_max_y_mid,
		x_min_y_max,
		x_mid_y_max,
		x_max_y_max
	};

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
