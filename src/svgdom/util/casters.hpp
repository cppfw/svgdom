/*
The MIT License (MIT)

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

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

#include <type_traits>

#include "../visitor.hpp"

namespace svgdom {

// TODO: doxygen all
template <class element_type>
class element_caster : public std::conditional_t<std::is_const_v<element_type>, const_visitor, visitor>
{
	static_assert(
		std::is_convertible_v<
			element_type*,
			typename std::conditional_t<std::is_const_v<element_type>, const element*, element*>>,
		"element_type must inherit element"
	);

public:
	element_type* pointer = nullptr;

	void visit(element_type& e) override
	{
		this->pointer = &e;
	}

	void default_visit(
		typename std::conditional_t<std::is_const_v<element_type>, const element&, element&> e,
		typename std::conditional_t<std::is_const_v<element_type>, const container&, container&> c
	) override
	{
		// do nothing
	}
};

template <bool is_const>
class container_caster_template : public std::conditional_t<is_const, const_visitor, visitor>
{
public:
	typename std::conditional_t<is_const, const container*, container*> pointer = nullptr;

	void default_visit(
		typename std::conditional_t<is_const, const element&, element&> e,
		typename std::conditional_t<is_const, const container&, container&> c
	) override
	{
		this->pointer = &c;
	}
};

using const_container_caster = container_caster_template<true>;
using container_caster = container_caster_template<false>;

inline container* cast_to_container(element* e)
{
	if (!e) {
		return nullptr;
	}
	container_caster caster;
	e->accept(caster);
	return caster.pointer;
}

inline const container* cast_to_container(const element* e)
{
	if (!e) {
		return nullptr;
	}
	const_container_caster caster;
	e->accept(caster);
	return caster.pointer;
}

template <bool is_const>
class styleable_caster_template : public std::conditional_t<is_const, const_visitor, visitor>
{
public:
	typename std::conditional_t<is_const, const styleable*, styleable*> pointer = nullptr;

	void visit(typename std::conditional_t<is_const, const path_element&, path_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const rect_element&, rect_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const circle_element&, circle_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const ellipse_element&, ellipse_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const line_element&, line_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const polyline_element&, polyline_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const polygon_element&, polygon_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const g_element&, g_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const svg_element&, svg_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const symbol_element&, symbol_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const use_element&, use_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const defs_element&, defs_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const gradient::stop_element&, gradient::stop_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(
		typename std::conditional_t<is_const, const linear_gradient_element&, linear_gradient_element&> e
	) override
	{
		this->pointer = &e;
	}

	void visit(
		typename std::conditional_t<is_const, const radial_gradient_element&, radial_gradient_element&> e
	) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const filter_element&, filter_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(
		typename std::conditional_t<is_const, const fe_gaussian_blur_element&, fe_gaussian_blur_element&> e
	) override
	{
		this->pointer = &e;
	}

	void visit(
		typename std::conditional_t<is_const, const fe_color_matrix_element&, fe_color_matrix_element&> e
	) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const fe_blend_element&, fe_blend_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const fe_composite_element&, fe_composite_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const image_element&, image_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const mask_element&, mask_element&> e) override
	{
		this->pointer = &e;
	}

	void visit(typename std::conditional_t<is_const, const text_element&, text_element&> e) override
	{
		this->pointer = &e;
	}
};

using const_styleable_caster = styleable_caster_template<true>;
using styleable_caster = styleable_caster_template<false>;

inline styleable* cast_to_styleable(element* e)
{
	if (!e) {
		return nullptr;
	}
	styleable_caster caster;
	e->accept(caster);
	return caster.pointer;
}

inline const styleable* cast_to_styleable(const element* e)
{
	if (!e) {
		return nullptr;
	}
	const_styleable_caster caster;
	e->accept(caster);
	return caster.pointer;
}

} // namespace svgdom
