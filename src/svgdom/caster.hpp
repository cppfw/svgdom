#pragma once

#include "visitor.hpp"

#include <type_traits>

namespace svgdom{

template <class T> class caster :
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

}
