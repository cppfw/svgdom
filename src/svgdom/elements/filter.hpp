#pragma once

#include <array>

#include "element.hpp"
#include "styleable.hpp"
#include "rectangle.hpp"
#include "referencing.hpp"
#include "container.hpp"
#include "coordinate_units.hpp"

namespace svgdom{
struct filter_element :
		public element,
		public styleable,
		public rectangle,
		public referencing,
		public container
{
	coordinate_units filter_units = coordinate_units::object_bounding_box;
	coordinate_units primitive_units = coordinate_units::user_space_on_use;

	// TODO: deprecated, remove.
	decltype(filter_units)& filterUnits = filter_units;

	// TODO: deprecated, remove.
	decltype(primitive_units)& primitiveUnits = primitive_units;
	
	// NOTE: filterRes attribute is dropped, it seems deprecated.
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef filter_element FilterElement;

struct filter_primitive :
		public element,
		public rectangle,
		public styleable
{
	std::string result;
};

// TODO: deprecated, remove.
typedef filter_primitive FilterPrimitive;

struct inputable{
	std::string in;
};

// TODO: deprecated, remove.
typedef inputable Inputable;

struct second_inputable{
	std::string in2;
};

// TODO: deprecated, remove.
typedef second_inputable SecondInputable;

struct fe_gaussian_blur_element :
		public filter_primitive,
		public inputable
{
	std::array<real, 2> std_deviation = {{-1, -1}};

	// TODO: deprecated, remove.
	decltype(std_deviation)& stdDeviation = std_deviation;
	
	/**
	 * @brief Tells if stdDeviation attribute is specified in XML.
	 * @return true if stdDeviation attribute is specified in XML.
	 * @return false otherwise.
	 */
	bool is_std_deviation_specified()const noexcept{
		return this->std_deviation[0] >= 0;
	}

	// TODO: deprecated, remove.
	bool isStdDeviationSpecified()const noexcept{
		return this->is_std_deviation_specified();
	}
	
	/**
	 * @brief Tells if two numbers are specified for stdDeviation attribute.
	 * @return true if two numbers are specified for stdDeviation attribute in XML.
	 * @return false otherwise.
	 */
	bool is_std_deviation_y_specified()const noexcept{
		return this->stdDeviation[1] >= 0;
	}

	// TODO: deprecated, remove.
	bool isStdDeviationYSpecified()const noexcept{
		return this->is_std_deviation_y_specified();
	}
	
	std::array<real, 2> get_std_deviation()const noexcept;

	// TODO: deprecated, remove.
	std::array<real, 2> getStdDeviation()const noexcept{
		return this->get_std_deviation();
	}
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef fe_gaussian_blur_element FeGaussianBlurElement;

struct fe_color_matrix_element :
		public filter_primitive,
		public inputable
{
	enum class type{
		matrix,
		MATRIX = matrix, // TODO: deprecated, remove.
		saturate,
		SATURATE = saturate, // TODO: deprecated, remove.
		hue_rotate,
		HUE_ROTATE = hue_rotate, // TODO: deprecated, remove.
		luminance_to_alpha,
		LUMINANCE_TO_ALPHA = luminance_to_alpha // TODO: deprecated, remove.
	};

	type type_ = type::matrix;
	
	// TODO: deprecated, remove.
	typedef type Type_e;

	std::array<real, 20> values;
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef fe_color_matrix_element FeColorMatrixElement;

struct fe_blend_element :
		public filter_primitive,
		public inputable,
		public second_inputable
{
	enum class mode{
		normal,
		NORMAL = normal, // TODO: deprecated, remove.
		multiply,
		MULTIPLY = multiply, // TODO: deprecated, remove.
		screen,
		SCREEN = screen, // TODO: deprecated, remove.
		darken,
		DARKEN = darken, // TODO: deprecated, remove.
		lighten,
		LIGHTEN = lighten // TODO: deprecated, remove.
	} mode_ = mode::normal;

	// TODO: deprecated, remove.
	typedef mode Mode_e;
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef fe_blend_element FeBlendElement;

struct fe_composite_element :
		public filter_primitive,
		public inputable,
		public second_inputable
{
	enum class operator_{
		over,
		OVER = over, // TODO: deprecated, remove.
		in,
		IN = in, // TODO: deprecated, remove.
		out,
#ifdef OUT // on Windows somebody defines OUT macro
#	undef OUT
#endif
		OUT = out, // TODO: deprecated, remove.
		atop,
		ATOP = atop, // TODO: deprecated, remove.
		xor_,
		XOR = xor_, // TODO: deprecated, remove.
		arithmetic,
		ARITHMETIC = arithmetic // TODO: deprecated, remove.
	} operator__ = operator_::over;
	
	// TODO: deprecated, remove.
	typedef operator_ Operator_e;

	real k1, k2, k3, k4;
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef fe_composite_element FeCompositeElement;

}
