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

	const std::string& get_id()const override{
		return this->id;
	}

	utki::span<const std::string> get_classes()const override{
		return utki::make_span(this->classes);
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

}
