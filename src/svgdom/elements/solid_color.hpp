/*
The MIT License (MIT)

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#pragma once

#include "element.hpp"
#include "styleable.hpp"

namespace svgdom {

using namespace std::string_view_literals;

/**
 * @brief The 'solidColor' element.
 * The 'solidColor' element is a paint server element which provides a single color with opacity.
 * It is used as a paint server to fill or stroke objects with a solid color.
 *
 * The color is specified by the 'solid-color' style property (the initial value is 'black')
 * and the opacity is specified by the 'solid-opacity' style property (the initial value is 1).
 * Both can be given either as presentation attributes ('solid-color="..."' / 'solid-opacity="..."')
 * or inside the inline 'style' attribute. Use the corresponding methods of the 'styleable'
 * base class to query them, i.e. 'get_style_property' or 'get_presentation_attribute' with the
 * 'style_property::solid_color' and 'style_property::solid_opacity' style properties.
 *
 * This element was introduced in the SVG 2 specification (ported from SVG 1.2 Tiny).
 * It is not part of SVG 1.1, but is supported here for convenience.
 */
struct solid_color_element :
	public element, //
	public styleable //
{
	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	std::string_view get_id() const override
	{
		return this->id;
	}

	constexpr static std::string_view tag = "solidColor"sv;

	std::string_view get_tag() const override
	{
		return tag;
	}
};

} // namespace svgdom
