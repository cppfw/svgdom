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
		void parse(std::string_view str);
	} preserve_aspect_ratio;
};

}
