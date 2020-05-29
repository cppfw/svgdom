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

}
