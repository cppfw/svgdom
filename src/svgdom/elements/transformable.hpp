#pragma once

#include <vector>
#include <string>

#include "../config.hpp"

namespace svgdom{

/**
 * @brief An element which has 'transform' attribute or similar.
 */
struct transformable{
	struct transformation{
		enum class type{
			matrix,
			translate,
			scale,
			rotate,
			skewx,
			skewy
		};
		
		type type_;

		union{
			real a;
			real angle;
		};

		union{
			real b;
			real x;
		};

		union{
			real c;
			real y;
		};
		
		real d, e, f;
	};

	std::vector<transformation> transformations;
	
	std::string transformations_to_string()const;
	
	static decltype(transformable::transformations) parse(std::string_view str);
};

}
