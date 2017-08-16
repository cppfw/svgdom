#pragma once

#include "Element.hpp"
#include "Styleable.hpp"
#include "Transformable.hpp"
#include "Rectangle.hpp"
#include "Referencing.hpp"
#include "AspectRatioed.hpp"

namespace svgdom{
struct ImageElement :
		public Element,
		public Styleable,
		public Transformable,
		public Rectangle,
		public Referencing,
		public AspectRatioed
{
	void accept(Visitor& visitor) override;
	void accept(ConstVisitor& visitor) const override;
};
}