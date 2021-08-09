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

#include "../visitor.hpp"

#include <type_traits>

namespace svgdom{

// TODO: doxygen all
template <class T> class element_caster :
		public std::conditional<std::is_const<T>::value, const_visitor, visitor>::type
{
	static_assert(
			std::is_convertible<
					T*,
					typename std::conditional<
							std::is_const<T>::value,
							const element*,
							element*
						>::type
				>::value,
			"T must inherit element"
		);
public:
	T* pointer = nullptr;

	void visit(T& e)override{
		this->pointer = &e;
	}

	void default_visit(
			typename std::conditional<std::is_const<T>::value, const element&, element&>::type e,
			typename std::conditional<std::is_const<T>::value, const container&, container&>::type c
		)override
	{
		// do nothing
	}
};

template <bool Const> class container_caster_template :
		public std::conditional<Const, const_visitor, visitor>::type
{
public:
	typename std::conditional<Const, const container*, container*>::type pointer = nullptr;

	void default_visit(
			typename std::conditional<Const, const element&, element&>::type e,
			typename std::conditional<Const, const container&, container&>::type c
		)override
	{
		this->pointer = &c;
	}
};

typedef container_caster_template<true> const_container_caster;
typedef container_caster_template<false> container_caster;

inline container* cast_to_container(element* e){
	if(!e){
		return nullptr;
	}
	container_caster caster;
	e->accept(caster);
	return caster.pointer;
}

inline const container* cast_to_container(const element* e){
	if(!e){
		return nullptr;
	}
	const_container_caster caster;
	e->accept(caster);
	return caster.pointer;
}

template <bool Const> class styleable_caster_template :
		public std::conditional<Const, const_visitor, visitor>::type
{
public:
	typename std::conditional<Const, const styleable*, styleable*>::type pointer = nullptr;

	virtual void visit(typename std::conditional<Const, const path_element&, path_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const rect_element&, rect_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const circle_element&, circle_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const ellipse_element&, ellipse_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const line_element&, line_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const polyline_element&, polyline_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const polygon_element&, polygon_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const g_element&, g_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const svg_element&, svg_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const symbol_element&, symbol_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const use_element&, use_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const defs_element&, defs_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const gradient::stop_element&, gradient::stop_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const linear_gradient_element&, linear_gradient_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const radial_gradient_element&, radial_gradient_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const filter_element&, filter_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const fe_gaussian_blur_element&, fe_gaussian_blur_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const fe_color_matrix_element&, fe_color_matrix_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const fe_blend_element&, fe_blend_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const fe_composite_element&, fe_composite_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const image_element&, image_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const mask_element&, mask_element&>::type e){
		this->pointer = &e;
	}
	virtual void visit(typename std::conditional<Const, const text_element&, text_element&>::type e){
		this->pointer = &e;
	}
};

typedef styleable_caster_template<true> const_styleable_caster;
typedef styleable_caster_template<false> styleable_caster;

inline styleable* cast_to_styleable(element* e){
	if(!e){
		return nullptr;
	}
	styleable_caster caster;
	e->accept(caster);
	return caster.pointer;
}

inline const styleable* cast_to_styleable(const element* e){
	if(!e){
		return nullptr;
	}
	const_styleable_caster caster;
	e->accept(caster);
	return caster.pointer;
}

}
