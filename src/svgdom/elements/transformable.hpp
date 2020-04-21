#pragma once

#include <vector>
#include <string>

#include "../config.hpp"

namespace svgdom{

/**
 * @brief An element which has 'transform' attribute or similar.
 */
struct transformable{

	enum class transformation_type{
		matrix,
		MATRIX = matrix, // TODO: deprecated, remove.
		translate,
		TRANSLATE = translate, // TODO: deprecated, remove.
		scale,
		SCALE = scale, // TODO: deprecated, remove.
		rotate,
		ROTATE = rotate, // TODO: deprecated, remove.
		skewx,
		SKEWX = skewx, // TODO: deprecated, remove.
		skewy,
		SKEWY = skewy // TODO: deprecated, remove.
	};

	struct transformation{
		transformation_type type;

		// TODO: deprecated, remove.
		typedef transformation_type Type_e;

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

	// TODO: deprecated, remove.
	typedef transformation Transformation;

	std::vector<transformation> transformations;
	
	std::string transformations_to_string()const;

	// TODO: deprecated, remove.
	std::string transformationsToString()const{
		return this->transformations_to_string();
	}
	
	static decltype(transformable::transformations) parse(const std::string& str);
};

// TODO: derprecated, remove.
typedef transformable Transformable;

}
