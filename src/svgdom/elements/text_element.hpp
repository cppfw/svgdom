/*
The MIT License (MIT)

Copyright (c) 2015-2023 Ivan Gagis <igagis@gmail.com>

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

#include "container.hpp"
#include "element.hpp"
#include "styleable.hpp"
#include "transformable.hpp"

namespace svgdom {

using namespace std::string_view_literals;

class text_positioning
{
public:
	// TODO: attributes x, y, dx, dy, rotate are not implemented yet.
};

// TODO: why lint complains here on macos?
// NOLINTNEXTLINE(bugprone-exception-escape, "error: an exception may be thrown in function")
class text_element : public element, public container, public styleable, public transformable, public text_positioning
{
public:
	// TODO: attributes lengthAdjust, textLength are not implemented yet.

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	std::string_view get_id() const override
	{
		return this->id;
	}

	constexpr static std::string_view tag = "text"sv;

	std::string_view get_tag() const override
	{
		return std::string_view(tag);
	}
};

} // namespace svgdom
