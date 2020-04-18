#pragma once

#include <vector>

#include "elements/Styleable.hpp"

namespace svgdom{
class style_stack{
public:
	std::vector<const svgdom::Styleable*> stack;
	
	const svgdom::StyleValue* get_style_property(svgdom::StyleProperty_e p)const;

	// TODO: deprecated, remove.
	const svgdom::StyleValue* getStyleProperty(svgdom::StyleProperty_e p)const{
		return this->get_style_property(p);
	}
	
	class push{
		style_stack& ss;
	public:
		push(style_stack& ss, const svgdom::Styleable& s);
		~push()noexcept;
	};

	// TODO: deprecated, remove.
	typedef push Push;
};

// TODO: deprecated, remove.
typedef style_stack StyleStack;

}
