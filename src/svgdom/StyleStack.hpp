#pragma once

#include <vector>

#include "elements/Styleable.hpp"

namespace svgdom{
class StyleStack{
public:
	std::vector<const svgdom::Styleable*> stack;
	
	const svgdom::StyleValue* getStyleProperty(svgdom::StyleProperty_e p)const;
	
	class Push{
		StyleStack& ss;
	public:
		Push(StyleStack& ss, const svgdom::Styleable& s);
		~Push()noexcept;
	};
};
}
