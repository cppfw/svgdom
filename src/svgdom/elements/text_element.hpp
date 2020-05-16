#pragma once

#include "element.hpp"
#include "container.hpp"
#include "styleable.hpp"
#include "transformable.hpp"

namespace svgdom{

class text_positioning{
public:
	//TODO: attributes x, y, dx, dy, rotate are not implemented yet.
};

// TODO: deprecated, remove.
typedef text_positioning TextPositioning;

class text_element :
		public element,
		public container,
		public styleable,
		public transformable,
		public text_positioning
{
public:
	//TODO: attributes lengthAdjust, textLength are not implemented yet.
	
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef text_element TextElement;

}
