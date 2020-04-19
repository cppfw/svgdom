#pragma once

#include "element.hpp"
#include "Styleable.hpp"
#include "Transformable.hpp"
#include "rectangle.hpp"
#include "Referencing.hpp"
#include "aspect_ratioed.hpp"

namespace svgdom{
struct image_element :
		public element,
		public Styleable,
		public Transformable,
		public rectangle,
		public Referencing,
		public aspect_ratioed
{
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef image_element ImageElement;

}