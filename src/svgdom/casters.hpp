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
};

template <bool Const> class container_caster_template :
		public std::conditional<Const, const_visitor, visitor>::type
{
public:
	typename std::conditional<Const, const container*, container*>::type pointer = nullptr;

	void visit(typename std::conditional<Const, const svg_element&, svg_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const g_element&, g_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const filter_element&, filter_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const linear_gradient_element&, linear_gradient_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const radial_gradient_element&, radial_gradient_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const defs_element&, defs_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const symbol_element&, symbol_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const mask_element&, mask_element&>::type e)override{
		this->pointer = &e;
	}

	void visit(typename std::conditional<Const, const text_element&, text_element&>::type e)override{
		this->pointer = &e;
	}
};

typedef container_caster_template<true> const_container_caster;
typedef container_caster_template<false> container_caster;

}
