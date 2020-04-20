#pragma once

#include "element.hpp"
#include "styleable.hpp"
#include "Transformable.hpp"
#include "rectangle.hpp"
#include "referencing.hpp"
#include "aspect_ratioed.hpp"

namespace svgdom{
struct image_element :
		public element,
		public styleable,
		public Transformable,
		public rectangle,
		public referencing,
		public aspect_ratioed
{
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef image_element ImageElement;

}