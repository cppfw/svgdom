#pragma once

#include <vector>
#include <string>

#include "../config.hpp"

namespace svgdom{

/**
 * @brief An element which has 'transform' attribute or similar.
 */
struct Transformable{
	struct Transformation{
		enum class Type_e{
			MATRIX,
			TRANSLATE,
			SCALE,
			ROTATE,
			SKEWX,
			SKEWY
		} type;
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
	std::vector<Transformation> transformations;
	
	std::string transformationsToString()const;
	
	static decltype(Transformable::transformations) parse(const std::string& str);
};

}
