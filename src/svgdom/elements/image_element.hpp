#pragma once

#include "element.hpp"
#include "styleable.hpp"
#include "transformable.hpp"
#include "rectangle.hpp"
#include "referencing.hpp"
#include "aspect_ratioed.hpp"

namespace svgdom{
struct image_element :
		public element,
		public styleable,
		public transformable,
		public rectangle,
		public referencing,
		public aspect_ratioed
{
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

}
