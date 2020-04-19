#pragma once

namespace svgdom{
enum class coordinate_units{
	unknown,
	UNKNOWN = unknown, // TODO: deprecated, remove. 
	user_space_on_use,
	USER_SPACE_ON_USE = user_space_on_use, // TODO: deprecated, remove. 
	object_bounding_box,
	OBJECT_BOUNDING_BOX = object_bounding_box // TODO: deprecated, remove. 
};

// TODO: deprecated, remove. 
typedef coordinate_units CoordinateUnits_e;

}
