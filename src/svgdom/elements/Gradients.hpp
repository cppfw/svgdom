#pragma once

#include "element.hpp"
#include "container.hpp"
#include "Referencing.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"
#include "coordinate_units.hpp"

namespace svgdom{

/**
 * @brief Common base for gradient elements.
 */
struct Gradient :
		public Element,
		public container,
		public Referencing,
		public Transformable,
		public Styleable
{
	enum class SpreadMethod_e{
		DEFAULT,
		PAD,
		REFLECT,
		REPEAT
	} spreadMethod = SpreadMethod_e::DEFAULT;
	
	coordinate_units units = coordinate_units::unknown;
	
	struct StopElement :
			public Element,
			public Styleable
	{
		real offset;
		
		void accept(visitor& v)override;
		void accept(const_visitor& v) const override;

	};
	
	std::string spreadMethodToString()const;
};

struct LinearGradientElement : public Gradient{
	length x1 = length::make(0, length_unit::unknown);
	length y1 = length::make(0, length_unit::unknown);
	length x2 = length::make(100, length_unit::unknown);
	length y2 = length::make(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct RadialGradientElement : public Gradient{
	Length cx = Length::make(50, length_unit::UNKNOWN);
	Length cy = Length::make(50, length_unit::UNKNOWN);
	Length r = Length::make(50, length_unit::UNKNOWN);
	Length fx = Length::make(50, length_unit::UNKNOWN);
	Length fy = Length::make(50, length_unit::UNKNOWN);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

}
