#pragma once

#include <vector>

#include "elements/styleable.hpp"
#include "elements/container.hpp"

namespace svgdom{
class style_stack{
	struct node{
		const svgdom::styleable& s;
		const svgdom::container* c = nullptr;
		size_t index = 0; // index in its parent of the next item in the stack

		node(const svgdom::styleable& s) :
				s(s)
		{}
	};

	std::vector<node> stack;

public:
	const svgdom::style_value* get_style_property(svgdom::style_property p)const;
	
	class push{
		style_stack& ss;
	public:
		push(style_stack& ss, const svgdom::styleable& s);
		push(style_stack& ss, const svgdom::styleable& s, const svgdom::container& c);
		~push()noexcept;
	};
};

}
