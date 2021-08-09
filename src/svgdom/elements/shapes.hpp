/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#pragma once

#include "transformable.hpp"
#include "styleable.hpp"
#include "element.hpp"
#include "rectangle.hpp"

#include <r4/vector.hpp>

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
	
	static decltype(path) parse(std::string_view str);
	
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

	static decltype(points) parse(std::string_view s);
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
