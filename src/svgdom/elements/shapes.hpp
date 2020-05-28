#pragma once

#include "transformable.hpp"
#include "styleable.hpp"
#include "element.hpp"
#include "rectangle.hpp"

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
};

// TODO: deprecated, remove.
typedef shape Shape;

struct path_element : public shape{
	struct step{
		enum class type{
			unknown,
			UNKNOWN = unknown, // TODO: deprecated, remove.
			close,
			CLOSE = close, // TODO: deprecated, remove.
			move_abs,
			MOVE_ABS = move_abs, // TODO: deprecated, remove.
			move_rel,
			MOVE_REL = move_rel, // TODO: deprecated, remove.
			line_abs,
			LINE_ABS = line_abs, // TODO: deprecated, remove.
			line_rel,
			LINE_REL = line_rel, // TODO: deprecated, remove.
			horizontal_line_abs,
			HORIZONTAL_LINE_ABS = horizontal_line_abs, // TODO: deprecated, remove.
			horizontal_line_rel,
			HORIZONTAL_LINE_REL = horizontal_line_rel, // TODO: deprecated, remove.
			vertical_line_abs,
			VERTICAL_LINE_ABS = vertical_line_abs, // TODO: deprecated, remove.
			vertical_line_rel,
			VERTICAL_LINE_REL = vertical_line_rel, // TODO: deprecated, remove.
			cubic_abs,
			CUBIC_ABS = cubic_abs, // TODO: deprecated, remove.
			cubic_rel,
			CUBIC_REL = cubic_rel, // TODO: deprecated, remove.
			cubic_smooth_abs,
			CUBIC_SMOOTH_ABS = cubic_smooth_abs, // TODO: deprecated, remove.
			cubic_smooth_rel,
			CUBIC_SMOOTH_REL = cubic_smooth_rel, // TODO: deprecated, remove.
			quadratic_abs,
			QUADRATIC_ABS = quadratic_abs, // TODO: deprecated, remove.
			quadratic_rel,
			QUADRATIC_REL = quadratic_rel, // TODO: deprecated, remove.
			quadratic_smooth_abs,
			QUADRATIC_SMOOTH_ABS = quadratic_smooth_abs, // TODO: deprecated, remove.
			quadratic_smooth_rel,
			QUADRATIC_SMOOTH_REL = quadratic_smooth_rel, // TODO: deprecated, remove.
			arc_abs,
			ARC_ABS = arc_abs, // TODO: deprecated, remove.
			arc_rel,
			ARC_REL = arc_rel // TODO: deprecated, remove.
		};
		
		type type_;
	
		// TODO: deprecated, remove.
		typedef type Type_e;

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
			real xAxisRotation; // TODO: deprecated, remove.
		};
		
		union{
			real y2;
			struct{
				bool large_arc;
				bool sweep;
			} flags;
		};
		
		static type char_to_type(char c);

		// TODO: deprecated, remove.
		static type charToType(char c){
			return char_to_type(c);
		}

		static char type_to_char(type t);

		// TODO: deprecated, remove.
		static char typeToChar(type t){
			return type_to_char(t);
		}
	};

	// TODO: deprecated, remove.
	typedef step Step;

	std::vector<step> path;
	
	std::string path_to_string()const;

	// TODO: deprecated, remove.
	std::string pathToString()const{
		return this->path_to_string();
	}
	
	static decltype(path) parse(const std::string& str);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef path_element PathElement;

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

	// TODO: deprecated, remove.
	static rectangle rectangleDefaultValues;
};

// TODO: deprecated, remove.
typedef rect_element RectElement;

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

// TODO: deprecated, remove.
typedef circle_element CircleElement;

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

// TODO: deprecated, remove.
typedef ellipse_element EllipseElement;

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

//TODO: deprecated, remove.
typedef line_element LineElement;

struct polyline_shape : public shape{
	std::vector<std::array<real, 2>> points;
	
	std::string points_to_string()const;

	// TODO: deprecated, remove.
	std::string pointsToString()const{
		return this->points_to_string();
	}
	
	static decltype(points) parse(const std::string& str);
};

// TODO: deprecated, remove.
typedef polyline_shape PolylineShape;

struct polyline_element : public polyline_shape{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

//TODO: deprecated, remove.
typedef polyline_element PolylineElement;

struct polygon_element : public polyline_shape{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef polygon_element PolygonElement;

}
