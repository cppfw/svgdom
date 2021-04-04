#pragma once

#include "element.hpp"
#include "styleable.hpp"

#include <cssom/om.hpp>

#include <string>

namespace svgdom{

struct style_element : public element{
	cssom::sheet css;

	struct css_style_value : public cssom::property_value_base{
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
