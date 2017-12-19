#pragma once

#include <array>

#include "Element.hpp"
#include "Styleable.hpp"
#include "Rectangle.hpp"
#include "Referencing.hpp"
#include "Container.hpp"
#include "CoordinateUnits.hpp"

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
	
	//NOTE: filterRes attribute is dropped, it seems deprecated.
	
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};

struct FilterPrimitive :
		public Element,
		public Rectangle,
		public Styleable
{
	std::string result;
};

struct Inputable{
	std::string in;
};

struct SecondInputable{
	std::string in2;
};

struct FeGaussianBlurElement :
		public FilterPrimitive,
		public Inputable
{
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
	
	std::array<real, 2> getStdDeviation()const noexcept;
	
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};

struct FeColorMatrixElement :
		public FilterPrimitive,
		public Inputable
{
	enum class Type_e{
		MATRIX,
		SATURATE,
		HUE_ROTATE,
		LUMINANCE_TO_ALPHA
	} type = Type_e::MATRIX;
	
	std::array<real, 20> values;
	
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};

struct FeBlendElement :
		public FilterPrimitive,
		public Inputable,
		public SecondInputable
{
	enum class Mode_e{
		NORMAL,
		MULTIPLY,
		SCREEN,
		DARKEN,
		LIGHTEN
	} mode = Mode_e::NORMAL;
	
	void accept(Visitor& visitor) override;
	void accept(ConstVisitor& visitor) const override;
};

struct FeCompositeElement :
		public FilterPrimitive,
		public Inputable,
		public SecondInputable
{
	enum class Operator_e{
		OVER,
		IN,
#ifdef OUT //on Windows somebody defines OUT macro
#	undef OUT
#endif
		OUT,
		ATOP,
		XOR,
		ARITHMETIC
	} operator_v = Operator_e::OVER;
	
	real k1, k2, k3, k4;
	
	void accept(Visitor& visitor) override;
	void accept(ConstVisitor& visitor) const override;
};

}
