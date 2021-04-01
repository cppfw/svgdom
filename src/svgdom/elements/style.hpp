#pragma once

#include "element.hpp"
#include "styleable.hpp"

#include <cssdom/dom.hpp>

#include <string>

namespace svgdom{

struct style_element : public element{
	cssdom::document css;

	struct css_style_value : public cssdom::property_value_base{
		style_value value;
	};

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

}
