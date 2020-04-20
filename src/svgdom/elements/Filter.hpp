#pragma once

#include <array>

#include "element.hpp"
#include "Styleable.hpp"
#include "rectangle.hpp"
#include "referencing.hpp"
#include "container.hpp"
#include "coordinate_units.hpp"

namespace svgdom{
struct FilterElement :
		public element,
		public Styleable,
		public rectangle,
		public referencing,
		public container
{
	coordinate_units filterUnits = coordinate_units::object_bounding_box;
	coordinate_units primitiveUnits = coordinate_units::user_space_on_use;
	
	//NOTE: filterRes attribute is dropped, it seems deprecated.
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct FilterPrimitive :
		public element,
		public rectangle,
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
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
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
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
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
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
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
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

}
