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
		public Container,
		public Referencing,
		public Styleable,
		public Transformable
{
	enum class SpreadMethod_e{
		DEFAULT,
		PAD,
		REFLECT,
		REPEAT
	} spreadMethod = SpreadMethod_e::DEFAULT;
	
	//TODO: remove when svgren is refactored
	SpreadMethod_e getSpreadMethod()const noexcept;
	
	enum class Units_e{
		USER_SPACE_ON_USE,
		OBJECT_BOUNDING_BOX
	} units = Units_e::OBJECT_BOUNDING_BOX;
	
	bool isBoundingBoxUnits()const noexcept{
		return this->units == Units_e::OBJECT_BOUNDING_BOX;
	}
	
	struct StopElement :
			public Element,
			public Styleable
	{
		real offset;
		
		void toStream(std::ostream& s, unsigned indent)const override;
		
		void accept(Visitor& visitor) const override;
	};
	
	//TODO: remove when svgren is refactored
	const decltype(Container::children)& getStops()const noexcept;
	
	void attribsToStream(std::ostream& s)const;
};

struct LinearGradientElement : public Gradient{
	Length x1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length x2 = Length::make(100, Length::Unit_e::UNKNOWN);
	Length y2 = Length::make(0, Length::Unit_e::UNKNOWN);
	
	//TODO: remove when svgren is refactored
	Length getX1()const noexcept;
	Length getY1()const noexcept;
	Length getX2()const noexcept;
	Length getY2()const noexcept;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct RadialGradientElement : public Gradient{
	Length cx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length cy = Length::make(50, Length::Unit_e::UNKNOWN);
	Length r = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fy = Length::make(50, Length::Unit_e::UNKNOWN);
	
	//TODO: remove when svgren is refactored
	Length getCx()const noexcept;
	Length getCy()const noexcept;
	Length getR()const noexcept;
	
	/**
	 * Can return unit as UNKNOWN in that case fx should coincide with cx.
	 * @return Fx.
	 */
	//TODO: remove when svgren is refactored
	Length getFx()const noexcept;
	
	/**
	 * Can return unit as UNKNOWN in that case fy should coincide with cy.
	 * @return Fy.
	 */
	//TODO: remove when svgren is refactored
	Length getFy()const noexcept;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;

};

}
