#pragma once

#include "element.hpp"

#include <cssdom/dom.hpp>

#include <string>

namespace svgdom{

struct style_element : public element{
	cssdom::document css;

	static const std::string tag;

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;
};

}
