#pragma once

#include "Element.hpp"
#include "container.hpp"
#include "Styleable.hpp"
#include "Transformable.hpp"

namespace svgdom{

class TextPositioning{
public:
	//TODO: attributes x, y, dx, dy, rotate are not implemented yet.
};

class TextElement :
		public Element,
		public Container,
		public Styleable,
		public Transformable,
		public TextPositioning
{
public:
	//TODO: attributes lengthAdjust, textLength are not implemented yet.
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

}
