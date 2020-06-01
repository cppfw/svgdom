#pragma once

#include "visitor.hpp"

#include <type_traits>

namespace svgdom{

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

}
