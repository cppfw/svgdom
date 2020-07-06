#pragma once

#include "element.hpp"
#include "container.hpp"
#include "referencing.hpp"
#include "transformable.hpp"
#include "styleable.hpp"
#include "coordinate_units.hpp"

namespace svgdom{

/**
 * @brief Common base for gradient elements.
 */
struct gradient :
		public element,
		public container,
		public referencing,
		public transformable,
		public styleable
{
	enum class spread_method{
		default_,
		pad,
		reflect,
		repeat
	};

	spread_method spread_method_ = spread_method::default_;

	coordinate_units units = coordinate_units::unknown;
	
	struct stop_element :
			public element,
			public styleable
	{
		real offset;
		
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

	std::string spread_method_to_string()const;

	const std::string& get_id()const override{
		return this->id;
	}
};

struct linear_gradient_element : public gradient{
	length x1 = length(0, length_unit::unknown);
	length y1 = length(0, length_unit::unknown);
	length x2 = length(100, length_unit::unknown);
	length y2 = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct radial_gradient_element : public gradient{
	length cx = length(50, length_unit::unknown);
	length cy = length(50, length_unit::unknown);
	length r = length(50, length_unit::unknown);
	length fx = length(50, length_unit::unknown);
	length fy = length(50, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

}
