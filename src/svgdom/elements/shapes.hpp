#pragma once

#include "transformable.hpp"
#include "styleable.hpp"
#include "element.hpp"
#include "rectangle.hpp"

#include <r4/vector2.hpp>

namespace svgdom{

/**
 * @brief element representing a geometric shape.
 */
struct shape :
		public element,
		public transformable,
		public styleable
{
	const std::string& get_id()const override{
		return this->id;
	}

	utki::span<const std::string> get_classes()const override{
		return utki::make_span(this->classes);
	}
};

struct path_element : public shape{
	struct step{
		enum class type{
			unknown,
			close,
			move_abs,
			move_rel,
			line_abs,
			line_rel,
			horizontal_line_abs,
			horizontal_line_rel,
			vertical_line_abs,
			vertical_line_rel,
			cubic_abs,
			cubic_rel,
			cubic_smooth_abs,
			cubic_smooth_rel,
			quadratic_abs,
			quadratic_rel,
			quadratic_smooth_abs,
			quadratic_smooth_rel,
			arc_abs,
			arc_rel
		};
		
		type type_;
	
		real x, y;
		
		union{
			real x1;
			real rx;
		};
		
		union{
			real y1;
			real ry;
		};
		
		union{
			real x2;
			real x_axis_rotation;
		};
		
		union{
			real y2;
			struct{
				bool large_arc;
				bool sweep;
			} flags;
		};
		
		static type char_to_type(char c);

		static char type_to_char(type t);
	};

	std::vector<step> path;
	
	std::string path_to_string()const;
	
	static decltype(path) parse(const std::string& str);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct rect_element :
		public shape,
		public rectangle
{
	length rx = length(0, length_unit::unknown);
	length ry = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}

	static rectangle rectangle_default_values(){
		return rectangle(
				length(0, length_unit::number),
				length(0, length_unit::number),
				length(0, length_unit::unknown),
				length(0, length_unit::unknown)
			);
	}
};

struct circle_element : public shape{
	length cx = length(0, length_unit::unknown);
	length cy = length(0, length_unit::unknown);
	length r = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct ellipse_element : public shape{
	length cx = length(0, length_unit::unknown);
	length cy = length(0, length_unit::unknown);
	length rx = length(0, length_unit::unknown);
	length ry = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct line_element : public shape{
	length x1 = length(0, length_unit::unknown);
	length y1 = length(0, length_unit::unknown);
	length x2 = length(0, length_unit::unknown);
	length y2 = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct polyline_shape : public shape{
	std::vector<r4::vector2<real>> points;
	
	std::string points_to_string()const;

	static decltype(points) parse(const std::string& str);
};

struct polyline_element : public polyline_shape{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

struct polygon_element : public polyline_shape{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

}
