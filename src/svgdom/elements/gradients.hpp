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

#include <ratio>

#include "container.hpp"
#include "coordinate_units.hpp"
#include "element.hpp"
#include "referencing.hpp"
#include "styleable.hpp"
#include "transformable.hpp"

namespace svgdom {

using namespace std::string_view_literals;

/**
 * @brief Common base for gradient elements.
 */
// TODO: why lint complains here on macos?
// NOLINTNEXTLINE(bugprone-exception-escape, "error: an exception may be thrown in function")
struct gradient : public element, public container, public referencing, public transformable, public styleable {
	enum class spread_method {
		default_method,
		pad,
		reflect,
		repeat
	};

	spread_method spread_method_attribute = spread_method::default_method;

	coordinate_units units = coordinate_units::unknown;

	// TODO: why lint complains here on macos?
	// NOLINTNEXTLINE(bugprone-exception-escape, "error: an exception may be thrown in function")
	struct stop_element : public element, public styleable {
		real offset = 0;

		void accept(visitor& v) override;
		void accept(const_visitor& v) const override;

		std::string_view get_id() const override
		{
			return this->id;
		}

		constexpr static std::string_view tag = "stop"sv;

		std::string_view get_tag() const override
		{
			return tag;
		}
	};

	std::string spread_method_to_string() const;

	std::string_view get_id() const override
	{
		return this->id;
	}
};

struct linear_gradient_element : public gradient {
	// TODO: change default unit to percent?
	length x1 = length(0, length_unit::unknown);
	length y1 = length(0, length_unit::unknown);
	length x2 = length(std::centi::den, length_unit::unknown);
	length y2 = length(0, length_unit::unknown);

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	constexpr static std::string_view tag = "linearGradient"sv;

	std::string_view get_tag() const override
	{
		return tag;
	}
};

struct radial_gradient_element : public gradient {
	// TODO: change default unit to percent?
	length cx = length(real(std::centi::den) / 2, length_unit::unknown);
	length cy = length(real(std::centi::den) / 2, length_unit::unknown);
	length r = length(real(std::centi::den) / 2, length_unit::unknown);
	length fx = length(real(std::centi::den) / 2, length_unit::unknown);
	length fy = length(real(std::centi::den) / 2, length_unit::unknown);

	void accept(visitor& v) override;
	void accept(const_visitor& v) const override;

	constexpr static std::string_view tag = "radialGradient"sv;

	std::string_view get_tag() const override
	{
		return tag;
	}
};

} // namespace svgdom
