#pragma once

#include "Element.hpp"
#include "Container.hpp"
#include "Referencing.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"

namespace svgdom{

/**
 * @brief Common base for gradient elements.
 */
struct Gradient :
		public Element,
		public Container,
		public Referencing,
		public Transformable
{
	enum class SpreadMethod_e{
		DEFAULT,
		PAD,
		REFLECT,
		REPEAT
	} spreadMethod = SpreadMethod_e::DEFAULT;
	
	enum class Units_e{
		USER_SPACE_ON_USE,
		OBJECT_BOUNDING_BOX
	} units = Units_e::OBJECT_BOUNDING_BOX;
	
	bool isBoundingBoxUnits()const noexcept{
		return this->units == Units_e::OBJECT_BOUNDING_BOX;
	}
	
	struct StopElement : public Element{
		real offset;
		
		void accept(Visitor& visitor) const override;
	};
	
	std::string spreadMethodToString()const;
};

struct LinearGradientElement : public Gradient{
	Length x1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length x2 = Length::make(100, Length::Unit_e::UNKNOWN);
	Length y2 = Length::make(0, Length::Unit_e::UNKNOWN);
	
	void accept(Visitor& visitor) const override;
};

struct RadialGradientElement : public Gradient{
	Length cx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length cy = Length::make(50, Length::Unit_e::UNKNOWN);
	Length r = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fy = Length::make(50, Length::Unit_e::UNKNOWN);
	
	void accept(Visitor& visitor) const override;
};

}
