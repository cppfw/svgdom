#pragma once

#include <array>

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

struct FilterPrimitive :
		public Element,
		public Rectangle,
		public Styleable
{
	std::string result;
};

struct InputableFilterPrimitive : public FilterPrimitive{
	std::string in;
};

struct TwoInputableFilterPrimitive : public InputableFilterPrimitive{
	std::string in2;
};

struct FeGaussianBlurElement : public InputableFilterPrimitive{
	std::array<real, 2> stdDeviation = {{-1, -1}};
	
	/**
	 * @brief Tells if stdDeviation attribute is specified in XML.
	 * @return true if stdDeviation attribute is specified in XML.
	 * @return false otherwise.
	 */
	bool isStdDeviationSpecified()const noexcept{
		return this->stdDeviation[0] >= 0;
	}
	
	/**
	 * @brief Tells if two numbers are specified for stdDeviation attribute.
	 * @return true if two numbers are specified for stdDeviation attribute in XML.
	 * @return false otherwise.
	 */
	bool isStdDeviationYSpecified()const noexcept{
		return this->stdDeviation[1] >= 0;
	}
	
	void accept(Visitor& visitor) const override;
};

}
