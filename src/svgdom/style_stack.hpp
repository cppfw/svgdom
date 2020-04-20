#pragma once

#include <vector>

#include "elements/styleable.hpp"

namespace svgdom{
class style_stack{
public:
	std::vector<const svgdom::styleable*> stack;
	
	const svgdom::style_value* get_style_property(svgdom::style_property p)const;

	// TODO: deprecated, remove.
	const svgdom::style_value* getStyleProperty(svgdom::style_property p)const{
		return this->get_style_property(p);
	}
	
	class push{
		style_stack& ss;
	public:
		push(style_stack& ss, const svgdom::styleable& s);
		~push()noexcept;
	};

	// TODO: deprecated, remove.
	typedef push Push;
};

// TODO: deprecated, remove.
typedef style_stack StyleStack;

}
