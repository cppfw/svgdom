#pragma once

#include "Element.hpp"
#include "Styleable.hpp"
#include "Rectangle.hpp"
#include "Referencing.hpp"
#include "Container.hpp"
#include "../CoordinateUnits.hpp"

namespace svgdom{
struct FilterElement :
		public Element,
		public Styleable,
		public Rectangle,
		public Referencing,
		public Container
{
	CoordinateUnits_e filterUnits = CoordinateUnits_e::OBJECT_BOUNDING_BOX;
	CoordinateUnits_e primitiveUnits = CoordinateUnits_e::USER_SPACE_ON_USE;
	
	void accept(Visitor& visitor) const override;
};
}
