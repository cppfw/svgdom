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
struct gradient :
		public element,
		public container,
		public Referencing,
		public Transformable,
		public Styleable
{
	enum class spread_method_kind{
		default_kind,
		DEFAULT = default_kind, // TODO: deprecated, remove.
		pad,
		PAD = pad, // TODO: deprecated, remove.
		reflect,
		REFLECT = reflect, // TODO: deprecated, remove.
		repeat,
		REPEAT = repeat // TODO: deprecated, remove.
	} spread_method = spread_method_kind::default_kind;

	// TODO: deprecated, remove.
	spread_method_kind& spreadMethod = spread_method;

	// TODO: deprecated, remove.
	typedef spread_method_kind SpreadMethod_e;
	
	coordinate_units units = coordinate_units::unknown;
	
	struct stop_element :
			public element,
			public Styleable
	{
		real offset;
		
		void accept(visitor& v)override;
		void accept(const_visitor& v) const override;
	};

	// TODO: deprecated, remove.
	typedef stop_element StopElement;
	
	std::string spread_method_to_string()const;

	// TODO: deprecated, remove.
	std::string spreadMethodToString()const{
		return this->spread_method_to_string();
	}
};

struct linear_gradient_element : public gradient{
	length x1 = length::make(0, length_unit::unknown);
	length y1 = length::make(0, length_unit::unknown);
	length x2 = length::make(100, length_unit::unknown);
	length y2 = length::make(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct radial_gradient_element : public gradient{
	length cx = length::make(50, length_unit::unknown);
	length cy = length::make(50, length_unit::unknown);
	length r = length::make(50, length_unit::unknown);
	length fx = length::make(50, length_unit::unknown);
	length fy = length::make(50, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef gradient Gradient;

// TODO: deprecated, remove.
typedef linear_gradient_element LinearGradientElement;

// TODO: deprecated, remove.
typedef radial_gradient_element RadialGradientElement;

}
