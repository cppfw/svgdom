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

	// NOTE: filterRes attribute is dropped, it seems deprecated.
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct filter_primitive :
		public element,
		public rectangle,
		public styleable
{
	std::string result;

	const std::string& get_id()const override{
		return this->id;
	}
};

struct inputable{
	std::string in;
};

struct second_inputable{
	std::string in2;
};

struct fe_gaussian_blur_element :
		public filter_primitive,
		public inputable
{
	std::array<real, 2> std_deviation = {{-1, -1}};

	/**
	 * @brief Tells if stdDeviation attribute is specified in XML.
	 * @return true if stdDeviation attribute is specified in XML.
	 * @return false otherwise.
	 */
	bool is_std_deviation_specified()const noexcept{
		return this->std_deviation[0] >= 0;
	}
	
	/**
	 * @brief Tells if two numbers are specified for stdDeviation attribute.
	 * @return true if two numbers are specified for stdDeviation attribute in XML.
	 * @return false otherwise.
	 */
	bool is_std_deviation_y_specified()const noexcept{
		return this->std_deviation[1] >= 0;
	}

	std::array<real, 2> get_std_deviation()const noexcept;

	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct fe_color_matrix_element :
		public filter_primitive,
		public inputable
{
	enum class type{
		matrix,
		saturate,
		hue_rotate,
		luminance_to_alpha
	};

	type type_ = type::matrix;

	std::array<real, 20> values;
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct fe_blend_element :
		public filter_primitive,
		public inputable,
		public second_inputable
{
	enum class mode{
		normal,
		multiply,
		screen,
		darken,
		lighten
	} mode_ = mode::normal;

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
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

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef fe_composite_element FeCompositeElement;

}
