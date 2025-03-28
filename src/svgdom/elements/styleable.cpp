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

#include "styleable.hpp"

#include <array>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <ratio>
#include <set>

#include <utki/debug.hpp>
#include <utki/util.hpp>

#include "../malformed_svg_error.hpp"
#include "../util.hxx"

#include "element.hpp"

using namespace svgdom;
using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
constexpr const char* none_word = "none";
constexpr const char* inherit_word = "inherit";
constexpr const char* current_color_word = "currentColor";
} // namespace

namespace {
std::string stroke_dasharray_to_string(const style_value& v)
{
	// special values must be already handled by styleable::style_value_to_string() at this point
	ASSERT(!std::holds_alternative<style_value_special>(v), [&](auto& o) {
		o << "v = " << unsigned(*std::get_if<style_value_special>(&v));
	})

	if (!std::holds_alternative<std::vector<length>>(v)) {
		return {};
	}

	std::stringstream ss;

	auto dasharray = *std::get_if<std::vector<length>>(&v);

	for (auto i = dasharray.begin(); i != dasharray.end(); ++i) {
		if (i != dasharray.begin()) {
			ss << " ";
		}
		ss << (*i);
	}

	return ss.str();
}
} // namespace

namespace {
style_value parse_stroke_dasharray(std::string_view str)
{
	// the "inherit" case is handled already by styleable::parse_style_property_value()
	if (str == none_word) {
		return {style_value_special::none};
	}

	std::vector<length> dasharray;

	utki::string_parser p(str);

	p.skip_whitespaces();

	while (!p.empty()) {
		auto len_str = p.read_word_until(',');
		auto l = length::parse(len_str);
		dasharray.push_back(l);

		p.skip_whitespaces_and_comma();
	}

	return {std::move(dasharray)};
}
} // namespace

std::string styleable::style_value_to_string(style_property p, const style_value& v)
{
	if (!is_valid(v)) {
		return {};
	}

	if (is_inherit(v)) {
		return inherit_word;
	} else if (is_none(v)) {
		return none_word;
	} else if (is_current_color(v)) {
		return current_color_word;
	}

	std::stringstream s;
	switch (p) {
		default:
			LOG([&](auto& o) {
				o << "Unimplemented style property: " << styleable::property_to_string(p) << ", writing empty value."
				  << std::endl;
			})
			break;
		case style_property::color_interpolation_filters:
			s << color_interpolation_filters_to_string(v);
			break;
		case style_property::stroke_miterlimit:
		case style_property::stop_opacity:
		case style_property::opacity:
		case style_property::stroke_opacity:
		case style_property::fill_opacity:
			if (std::holds_alternative<real>(v)) {
				s << *std::get_if<real>(&v);
			}
			break;
		case style_property::stop_color:
		case style_property::fill:
		case style_property::stroke:
			s << paint_to_string(v);
			break;
		case style_property::stroke_dashoffset:
		case style_property::stroke_width:
			if (std::holds_alternative<length>(v)) {
				s << *std::get_if<length>(&v);
			}
			break;
		case style_property::stroke_linecap:
			if (std::holds_alternative<svgdom::stroke_line_cap>(v)) {
				switch (*std::get_if<svgdom::stroke_line_cap>(&v)) {
					default:
						ASSERT(false)
						break;
					case stroke_line_cap::butt:
						s << "butt";
						break;
					case stroke_line_cap::round:
						s << "round";
						break;
					case stroke_line_cap::square:
						s << "square";
						break;
				}
			}
			break;
		case style_property::stroke_linejoin:
			if (std::holds_alternative<svgdom::stroke_line_join>(v)) {
				switch (*std::get_if<svgdom::stroke_line_join>(&v)) {
					default:
						ASSERT(false)
						break;
					case stroke_line_join::miter:
						s << "miter";
						break;
					case stroke_line_join::round:
						s << "round";
						break;
					case stroke_line_join::bevel:
						s << "bevel";
						break;
				}
			}
			break;
		case style_property::fill_rule:
			if (std::holds_alternative<svgdom::fill_rule>(v)) {
				switch (*std::get_if<svgdom::fill_rule>(&v)) {
					default:
						ASSERT(false)
						break;
					case fill_rule::evenodd:
						s << "evenodd";
						break;
					case fill_rule::nonzero:
						s << "nonzero";
						break;
				}
			}
			break;
		case style_property::mask:
		case style_property::filter:
			if (std::holds_alternative<std::string>(v)) {
				s << "url(" << *std::get_if<std::string>(&v) << ")";
			}
			break;
		case style_property::display:
			s << display_to_string(v);
			break;
		case style_property::enable_background:
			s << enable_background_to_string(v);
			break;
		case style_property::visibility:
			s << visibility_to_string(v);
			break;
		case style_property::stroke_dasharray:
			s << stroke_dasharray_to_string(v);
			break;
	}
	return s.str();
}

std::string styleable::styles_to_string() const
{
	std::stringstream s;

	bool is_first = true;

	for (auto& st : this->styles) {
		if (is_first) {
			is_first = false;
		} else {
			s << "; ";
		}

		ASSERT(st.first != style_property::unknown)

		s << property_to_string(st.first) << ":";

		s << style_value_to_string(st.first, st.second);
	}
	return s.str();
}

std::string styleable::classes_to_string() const
{
	std::stringstream ss;

	for (auto i = this->classes.begin(); i != this->classes.end(); ++i) {
		if (i != this->classes.begin()) {
			ss << " ";
		}
		ss << *i;
	}

	return ss.str();
}

// input parameter 'str' should have no leading or trailing white spaces
style_value styleable::parse_style_property_value(style_property type, std::string_view str)
{
	if (str == inherit_word) {
		return {style_value_special::inherit};
	}

	switch (type) {
		default:
			LOG([&](auto& o) {
				o << "unimplemented style property encountered: " << styleable::property_to_string(type) << std::endl;
			})
			return {style_value_special::unknown};
		case style_property::color_interpolation_filters:
			return parse_color_interpolation(str);
		case style_property::stroke_miterlimit:
			{
				real miter_limit = utki::string_parser(str).read_number<real>();
				using std::max;
				miter_limit = max(miter_limit, real(1)); // minimal value is 1
				return {miter_limit};
			}
			break;
		case style_property::stop_opacity:
		case style_property::opacity:
		case style_property::stroke_opacity:
		case style_property::fill_opacity:
			{
				real opacity = utki::string_parser(str).read_number<real>();
				using std::min;
				using std::max;
				opacity = max(real(0), min(opacity, real(1))); // clamp to [0:1]
				return {opacity};
			}
			break;
		case style_property::stop_color:
		case style_property::fill:
		case style_property::stroke:
			return parse_paint(str);
		case style_property::stroke_dashoffset:
		case style_property::stroke_width:
			return length::parse(str);
		case style_property::stroke_linecap:
			{
				if (str == "butt") {
					return stroke_line_cap::butt;
				} else if (str == "round") {
					return stroke_line_cap::round;
				} else if (str == "square") {
					return stroke_line_cap::square;
				}

				LOG([&](auto& o) {
					o << "unknown strokeLineCap value:" << str << std::endl;
				})
				return {};
			}
		case style_property::stroke_linejoin:
			{
				if (str == "miter") {
					return stroke_line_join::miter;
				} else if (str == "round") {
					return stroke_line_join::round;
				} else if (str == "bevel") {
					return stroke_line_join::bevel;
				}

				LOG([&](auto& o) {
					o << "unknown strokeLineJoin value:" << str << std::endl;
				})
				return {};
			}
		case style_property::fill_rule:
			{
				if (str == "nonzero") {
					return fill_rule::nonzero;
				} else if (str == "evenodd") {
					return fill_rule::evenodd;
				}

				LOG([&](auto& o) {
					o << "unknown fill-rule value:" << str << std::endl;
				})
				return {};
			}
		case style_property::mask:
		case style_property::filter:
			return parse_url(str);
		case style_property::display:
			return parse_display(str);
		case style_property::enable_background:
			return parse_enable_background(str);
		case style_property::visibility:
			return parse_visibility(str);
		case style_property::stroke_dasharray:
			return parse_stroke_dasharray(str);
	}
}

style_value svgdom::parse_url(std::string_view str)
{
	const static std::string url_word = "url(";

	utki::string_parser p(str);
	p.skip_whitespaces();

	if (url_word != p.read_chars(url_word.size())) {
		return {style_value_special::unknown};
	}

	p.skip_whitespaces();
	auto url = p.read_word_until(')');

	p.skip_whitespaces();
	if (p.read_char() == ')') {
		return {std::string(url)};
	}

	return {style_value_special::unknown};
}

namespace {
style_value parse_style_property_value(style_property type, utki::string_parser& p)
{
	p.skip_whitespaces();
	auto str = p.read_chars_until(';');
	return styleable::parse_style_property_value(type, trim_tail(str));
}
} // namespace

decltype(styleable::styles) styleable::parse(const std::string& str)
{
	utki::string_parser p(str);

	p.skip_whitespaces();

	decltype(styleable::styles) ret;

	while (!p.empty()) {
		auto property = p.read_word_until(':');

		style_property type = styleable::string_to_property(property);

		if (type == style_property::unknown) {
			// unknown style property, skip it
			LOG([&](auto& o) {
				o << "Unknown style property: " << property << std::endl;
			})
			LOG([&](auto& o) {
				o << "str = " << str << std::endl;
			})
			LOG([&](auto& o) {
				o << "ret.size() = " << ret.size() << std::endl;
			})
			p.skip_inclusive_until(';');
			continue;
		}

		p.skip_whitespaces();

		if (p.read_char() != ':') {
			return ret; // expected colon
		}

		style_value v = ::parse_style_property_value(type, p);

		p.skip_whitespaces();

		if (!p.empty() && p.read_char() != ';') {
			return ret; // expected semicolon
		}

		ret[type] = std::move(v);

		p.skip_whitespaces();
	}

	return ret;
}

namespace {
const std::set<style_property> non_inheritable_style_properties = {
	style_property::alignment_baseline,
	style_property::baseline_shift,
	style_property::clip,
	style_property::clip_path,
	style_property::display,
	style_property::dominant_baseline,
	style_property::enable_background,
	style_property::filter,
	style_property::flood_color,
	style_property::flood_opacity,
	style_property::lighting_color,
	style_property::mask,
	style_property::opacity,
	style_property::overflow,
	style_property::stop_color,
	style_property::stop_opacity,
	style_property::text_decoration,
	style_property::unicode_bidi
};
} // namespace

bool styleable::is_inheritable(style_property p)
{
	return non_inheritable_style_properties.find(p) == non_inheritable_style_properties.end();
}

namespace {
const std::map<std::string_view, style_property> string_to_property_map = {
	{		  "alignment-baseline",           style_property::alignment_baseline},
	{			  "baseline-shift",               style_property::baseline_shift},
	{						"clip",						 style_property::clip},
	{				   "clip-path",					style_property::clip_path},
	{				   "clip-rule",					style_property::clip_rule},
	{					   "color",						style_property::color},
	{		 "color-interpolation",          style_property::color_interpolation},
	{ "color-interpolation-filters",  style_property::color_interpolation_filters},
	{			   "color-profile",                style_property::color_profile},
	{			 "color-rendering",              style_property::color_rendering},
	{					  "cursor",					   style_property::cursor},
	{				   "direction",					style_property::direction},
	{					 "display",					  style_property::display},
	{		   "dominant-baseline",            style_property::dominant_baseline},
	{		   "enable-background",            style_property::enable_background},
	{						"fill",						 style_property::fill},
	{				"fill-opacity",                 style_property::fill_opacity},
	{				   "fill-rule",					style_property::fill_rule},
	{					  "filter",					   style_property::filter},
	{				 "flood-color",                  style_property::flood_color},
	{			   "flood-opacity",                style_property::flood_opacity},
	{						"font",						 style_property::font},
	{				 "font-family",                  style_property::font_family},
	{				   "font-size",					style_property::font_size},
	{			"font-size-adjust",             style_property::font_size_adjust},
	{				"font-stretch",                 style_property::font_stretch},
	{				  "font-style",				   style_property::font_style},
	{				"font-variant",                 style_property::font_variant},
	{				 "font-weight",                  style_property::font_weight},
	{"glyph-orientation-horizontal", style_property::glyph_orientation_horizontal},
	{  "glyph-orientation-vertical",   style_property::glyph_orientation_vertical},
	{			 "image-rendering",              style_property::image_rendering},
	{					 "kerning",					  style_property::kerning},
	{			  "letter-spacing",               style_property::letter_spacing},
	{			  "lighting-color",               style_property::lighting_color},
	{					  "marker",					   style_property::marker},
	{				  "marker-end",				   style_property::marker_end},
	{				  "marker-mid",				   style_property::marker_mid},
	{				"marker-start",                 style_property::marker_start},
	{						"mask",						 style_property::mask},
	{					 "opacity",					  style_property::opacity},
	{					"overflow",					 style_property::overflow},
	{			  "pointer-events",               style_property::pointer_events},
	{			 "shape-rendering",              style_property::shape_rendering},
	{				  "stop-color",				   style_property::stop_color},
	{				"stop-opacity",                 style_property::stop_opacity},
	{					  "stroke",					   style_property::stroke},
	{			"stroke-dasharray",             style_property::stroke_dasharray},
	{		   "stroke-dashoffset",            style_property::stroke_dashoffset},
	{			  "stroke-linecap",               style_property::stroke_linecap},
	{			 "stroke-linejoin",              style_property::stroke_linejoin},
	{		   "stroke-miterlimit",            style_property::stroke_miterlimit},
	{			  "stroke-opacity",               style_property::stroke_opacity},
	{				"stroke-width",                 style_property::stroke_width},
	{				 "text-anchor",                  style_property::text_anchor},
	{			 "text-decoration",              style_property::text_decoration},
	{			  "text-rendering",               style_property::text_rendering},
	{				"unicode-bidi",                 style_property::unicode_bidi},
	{				  "visibility",				   style_property::visibility},
	{				"word-spacing",                 style_property::word_spacing},
	{				"writing-mode",                 style_property::writing_mode}
};
} // namespace

namespace {
const auto property_to_string_map = utki::flip_map(string_to_property_map);
} // namespace

style_property styleable::string_to_property(std::string_view str)
{
	auto i = string_to_property_map.find(str);
	if (i != string_to_property_map.end()) {
		return i->second;
	}

	return style_property::unknown;
}

std::string_view styleable::property_to_string(style_property p)
{
	auto i = property_to_string_map.find(p);
	if (i != property_to_string_map.end()) {
		return i->second;
	}
	return {};
}

const style_value* styleable::get_style_property(style_property p) const
{
	auto i = this->styles.find(p);
	if (i != this->styles.end()) {
		return &i->second;
	}
	return nullptr;
}

const style_value* styleable::get_presentation_attribute(style_property p) const
{
	auto i = this->presentation_attributes.find(p);
	if (i != this->presentation_attributes.end()) {
		return &i->second;
	}
	return nullptr;
}

r4::vector3<real> svgdom::get_rgb(const style_value& v)
{
	if (!std::holds_alternative<uint32_t>(v)) {
		return 0;
	}

	auto c = std::get<uint32_t>(v);

	r4::vector3<real> ret;

	ret.r() = real(c & utki::byte_mask) / real(utki::byte_mask);
	ret.g() = real((c >> utki::byte_bits) & utki::byte_mask) / real(utki::byte_mask);
	ret.b() = real((c >> (utki::byte_bits * 2)) & utki::byte_mask) / real(utki::byte_mask);

	return ret;
}

style_value svgdom::make_style_value(uint8_t r, uint8_t g, uint8_t b)
{
	return {uint32_t(r) | (uint32_t(g) << utki::byte_bits) | (uint32_t(b) << (utki::byte_bits * 2))};
}

style_value svgdom::make_style_value(const r4::vector3<real>& rgb)
{
	auto c = (rgb * utki::byte_mask).to<uint8_t>();
	return make_style_value(uint8_t(c.r()), uint8_t(c.g()), uint8_t(c.b()));
}

namespace {
const std::map<std::string_view, uint32_t> color_name_to_color_map = {
	{		   "aliceblue"sv, 0xfff8f0},
	{		"antiquewhite"sv, 0xd7ebfa},
	{				"aqua"sv, 0xffff00},
	{		  "aquamarine"sv, 0xd4ff7f},
	{			   "azure"sv, 0xfffff0},
	{			   "beige"sv, 0xdcf5f5},
	{			  "bisque"sv, 0xc4e4ff},
	{			   "black"sv,      0x0},
	{	  "blanchedalmond"sv, 0xcdebff},
	{				"blue"sv, 0xff0000},
	{		  "blueviolet"sv, 0xe22b8a},
	{			   "brown"sv, 0x2a2aa5},
	{		   "burlywood"sv, 0x87b8de},
	{		   "cadetblue"sv, 0xa09e5f},
	{		  "chartreuse"sv,   0xff7f},
	{		   "chocolate"sv, 0x1e69d2},
	{			   "coral"sv, 0x507fff},
	{	  "cornflowerblue"sv, 0xed9564},
	{			"cornsilk"sv, 0xdcf8ff},
	{			 "crimson"sv, 0x3c14dc},
	{				"cyan"sv, 0xffff00},
	{			"darkblue"sv, 0x8b0000},
	{			"darkcyan"sv, 0x8b8b00},
	{	   "darkgoldenrod"sv,  0xb86b8},
	{			"darkgray"sv, 0xa9a9a9},
	{		   "darkgreen"sv,   0x6400},
	{			"darkgrey"sv, 0xa9a9a9},
	{		   "darkkhaki"sv, 0x6bb7bd},
	{		 "darkmagenta"sv, 0x8b008b},
	{	  "darkolivegreen"sv, 0x2f6b55},
	{		  "darkorange"sv,   0x8cff},
	{		  "darkorchid"sv, 0xcc3299},
	{			 "darkred"sv,     0x8b},
	{		  "darksalmon"sv, 0x7a96e9},
	{		"darkseagreen"sv, 0x8fbc8f},
	{	   "darkslateblue"sv, 0x8b3d48},
	{	   "darkslategray"sv, 0x4f4f2f},
	{	   "darkslategrey"sv, 0x4f4f2f},
	{	   "darkturquoise"sv, 0xd1ce00},
	{		  "darkviolet"sv, 0xd30094},
	{			"deeppink"sv, 0x9314ff},
	{		 "deepskyblue"sv, 0xffbf00},
	{			 "dimgray"sv, 0x696969},
	{			 "dimgrey"sv, 0x696969},
	{		  "dodgerblue"sv, 0xff901e},
	{		   "firebrick"sv, 0x2222b2},
	{		 "floralwhite"sv, 0xf0faff},
	{		 "forestgreen"sv, 0x228b22},
	{			 "fuchsia"sv, 0xff00ff},
	{		   "gainsboro"sv, 0xdcdcdc},
	{		  "ghostwhite"sv, 0xfff8f8},
	{				"gold"sv,   0xd7ff},
	{		   "goldenrod"sv, 0x20a5da},
	{				"gray"sv, 0x808080},
	{				"grey"sv, 0x808080},
	{			   "green"sv,   0x8000},
	{		 "greenyellow"sv, 0x2fffad},
	{			"honeydew"sv, 0xf0fff0},
	{			 "hotpink"sv, 0xb469ff},
	{		   "indianred"sv, 0x5c5ccd},
	{			  "indigo"sv, 0x82004b},
	{			   "ivory"sv, 0xf0ffff},
	{			   "khaki"sv, 0x8ce6f0},
	{			"lavender"sv, 0xfae6e6},
	{	   "lavenderblush"sv, 0xf5f0ff},
	{		   "lawngreen"sv,   0xfc7c},
	{		"lemonchiffon"sv, 0xcdfaff},
	{		   "lightblue"sv, 0xe6d8ad},
	{		  "lightcoral"sv, 0x8080f0},
	{		   "lightcyan"sv, 0xffffe0},
	{"lightgoldenrodyellowsv", 0xd2fafa},
	{		   "lightgray"sv, 0xd3d3d3},
	{		  "lightgreen"sv, 0x90ee90},
	{		   "lightgrey"sv, 0xd3d3d3},
	{		   "lightpink"sv, 0xc1b6ff},
	{		 "lightsalmon"sv, 0x7aa0ff},
	{	   "lightseagreen"sv, 0xaab220},
	{		"lightskyblue"sv, 0xface87},
	{	  "lightslategray"sv, 0x998877},
	{	  "lightslategrey"sv, 0x998877},
	{	  "lightsteelblue"sv, 0xdec4b0},
	{		 "lightyellow"sv, 0xe0ffff},
	{				"lime"sv,   0xff00},
	{		   "limegreen"sv, 0x32cd32},
	{			   "linen"sv, 0xe6f0fa},
	{			 "magenta"sv, 0xff00ff},
	{			  "maroon"sv,     0x80},
	{    "mediumaquamarinesv", 0xaacd66},
	{		  "mediumblue"sv, 0xcd0000},
	{		"mediumorchid"sv, 0xd355ba},
	{		"mediumpurple"sv, 0xdb7093},
	{	  "mediumseagreen"sv, 0x71b33c},
	{	 "mediumslateblue"sv, 0xee687b},
	{   "mediumspringgreensv", 0x9afa00},
	{	 "mediumturquoise"sv, 0xccd148},
	{	 "mediumvioletred"sv, 0x8515c7},
	{		"midnightblue"sv, 0x701919},
	{		   "mintcream"sv, 0xfafff5},
	{		   "mistyrose"sv, 0xe1e4ff},
	{			"moccasin"sv, 0xb5e4ff},
	{		 "navajowhite"sv, 0xaddeff},
	{				"navy"sv, 0x800000},
	{			 "oldlace"sv, 0xe6f5fd},
	{			   "olive"sv,   0x8080},
	{		   "olivedrab"sv, 0x238e6b},
	{			  "orange"sv,   0xa5ff},
	{		   "orangered"sv,   0x45ff},
	{			  "orchid"sv, 0xd670da},
	{	   "palegoldenrod"sv, 0xaae8ee},
	{		   "palegreen"sv, 0x98fb98},
	{	   "paleturquoise"sv, 0xeeeeaf},
	{	   "palevioletred"sv, 0x9370db},
	{		  "papayawhip"sv, 0xd5efff},
	{		   "peachpuff"sv, 0xb9daff},
	{				"peru"sv, 0x3f85cd},
	{				"pink"sv, 0xcbc0ff},
	{				"plum"sv, 0xdda0dd},
	{		  "powderblue"sv, 0xe6e0b0},
	{			  "purple"sv, 0x800080},
	{				 "red"sv,     0xff},
	{		   "rosybrown"sv, 0x8f8fbc},
	{		   "royalblue"sv, 0xe16941},
	{		 "saddlebrown"sv, 0x13458b},
	{			  "salmon"sv, 0x7280fa},
	{		  "sandybrown"sv, 0x60a4f4},
	{			"seagreen"sv, 0x578b2e},
	{			"seashell"sv, 0xeef5ff},
	{			  "sienna"sv, 0x2d52a0},
	{			  "silver"sv, 0xc0c0c0},
	{			 "skyblue"sv, 0xebce87},
	{		   "slateblue"sv, 0xcd5a6a},
	{		   "slategray"sv, 0x908070},
	{		   "slategrey"sv, 0x908070},
	{				"snow"sv, 0xfafaff},
	{		 "springgreen"sv, 0x7fff00},
	{		   "steelblue"sv, 0xb48246},
	{				 "tan"sv, 0x8cb4d2},
	{				"teal"sv, 0x808000},
	{			 "thistle"sv, 0xd8bfd8},
	{			  "tomato"sv, 0x4763ff},
	{		   "turquoise"sv, 0xd0e040},
	{			  "violet"sv, 0xee82ee},
	{			   "wheat"sv, 0xb3def5},
	{			   "white"sv, 0xffffff},
	{		  "whitesmoke"sv, 0xf5f5f5},
	{			  "yellow"sv,   0xffff},
	{		 "yellowgreen"sv, 0x32cd9a}
};
} // namespace

namespace {
const auto color_to_color_name_map = utki::flip_map(color_name_to_color_map);
} // namespace

namespace {
const std::map<std::string_view, display> string_to_display_map = {
	{			"inline",     svgdom::display::inline_display},
	{			 "block",			  svgdom::display::block},
	{		 "list-item",          svgdom::display::list_item},
	{			"run-in",             svgdom::display::run_in},
	{		   "compact",            svgdom::display::compact},
	{			"marker",             svgdom::display::marker},
	{			 "table",			  svgdom::display::table},
	{	  "inline-table",       svgdom::display::inline_table},
	{   "table-row-group",    svgdom::display::table_row_group},
	{"table-header-group", svgdom::display::table_header_group},
	{"table-footer-group", svgdom::display::table_footer_group},
	{		 "table-row",          svgdom::display::table_row},
	{"table-column-group", svgdom::display::table_column_group},
	{	  "table-column",       svgdom::display::table_column},
	{		"table-cell",         svgdom::display::table_cell},
	{	 "table-caption",      svgdom::display::table_caption},
	{		   none_word,               svgdom::display::none}
};
} // namespace

namespace {
const auto display_to_string_map = utki::flip_map(string_to_display_map);
} // namespace

style_value svgdom::parse_display(std::string_view& str)
{
	// NOTE: "inherit" is already checked on upper level.

	auto i = string_to_display_map.find(str);
	if (i == string_to_display_map.end()) {
		return {svgdom::display::inline_display}; // default value
	} else {
		return {i->second};
	}
}

std::string_view svgdom::display_to_string(const style_value& v)
{
	const auto& default_value = []() {
		auto i = display_to_string_map.find(svgdom::display::inline_display);
		ASSERT(i != display_to_string_map.end())
		return i->second;
	}();

	if (!std::holds_alternative<svgdom::display>(v)) {
		return default_value;
	}

	auto i = display_to_string_map.find(*std::get_if<svgdom::display>(&v));
	if (i == display_to_string_map.end()) {
		return default_value;
	}
	return i->second;
}

namespace {
const std::map<std::string_view, visibility> string_to_visibility_map = {
	{ "visible",  visibility::visible},
	{  "hidden",   visibility::hidden},
	{"collapse", visibility::collapse}
};
} // namespace

namespace {
const auto visibility_to_string_map = utki::flip_map(string_to_visibility_map);
} // namespace

style_value svgdom::parse_visibility(std::string_view str)
{
	// NOTE: "inherit" is already checked on upper level.

	auto i = string_to_visibility_map.find(str);
	if (i == string_to_visibility_map.end()) {
		return {svgdom::visibility::visible}; // default value
	} else {
		return {i->second};
	}
}

std::string_view svgdom::visibility_to_string(const style_value& v)
{
	const auto& default_value = []() {
		auto i = visibility_to_string_map.find(svgdom::visibility::visible);
		ASSERT(i != visibility_to_string_map.end())
		return i->second;
	}();

	if (!std::holds_alternative<svgdom::visibility>(v)) {
		return default_value;
	}

	auto i = visibility_to_string_map.find(*std::get_if<svgdom::visibility>(&v));
	if (i == visibility_to_string_map.end()) {
		return default_value;
	}
	return i->second;
}

style_value svgdom::parse_color_interpolation(std::string_view str)
{
	if (str == "auto") {
		return color_interpolation::automatic;
	} else if (str == "linearRGB") {
		return color_interpolation::linear_rgb;
	} else if (str == "sRGB") {
		return color_interpolation::s_rgb;
	}

	return {style_value_special::unknown};
}

namespace {
std::string to_string(color_interpolation ci)
{
	switch (ci) {
		case color_interpolation::automatic:
			return "auto";
		case color_interpolation::linear_rgb:
			return "linearRGB";
		case color_interpolation::s_rgb:
			return "sRGB";
		default:
			ASSERT(false, [&](auto& o) {
				o << "ci = " << unsigned(ci);
			})
			return "";
	}
}
} // namespace

std::string svgdom::color_interpolation_filters_to_string(const style_value& v)
{
	if (!std::holds_alternative<svgdom::color_interpolation>(v)) {
		return {};
	}

	return to_string(*std::get_if<svgdom::color_interpolation>(&v));
}

namespace {
decltype(enable_background_property::rect) parse_enable_background_new_rect(std::string_view str)
{
	using ret_type = decltype(enable_background_property::rect);

	try {
		utki::string_parser p(str);
		p.skip_inclusive_until(' '); // skip 'new'

		p.skip_whitespaces();

		if (p.empty()) {
			// indicate that rectangle is not specified
			return {
				{-1, -1},
				{-1, -1}
			};
		}

		return ret_type({p.read_number<real>(), p.read_number<real>()}, {p.read_number<real>(), p.read_number<real>()});
	} catch (std::invalid_argument&) {
		throw malformed_svg_error("malformed enable-background NEW string");
	}
}
} // namespace

style_value svgdom::parse_enable_background(std::string_view str)
{
	constexpr auto default_value = svgdom::enable_background::accumulate;

	std::string new_str = "new";
	if (str.substr(0, new_str.length()) == new_str) {
		try {
			return enable_background_property{
				svgdom::enable_background::new_background,
				parse_enable_background_new_rect(str)
			};
		} catch (malformed_svg_error&) {
			return enable_background_property{default_value};
		}
	}

	return enable_background_property{default_value};
}

std::string svgdom::enable_background_to_string(const style_value& v)
{
	const std::string& default_value = "accumulate";

	if (!std::holds_alternative<svgdom::enable_background_property>(v)) {
		return default_value;
	}

	auto ebp = *std::get_if<svgdom::enable_background_property>(&v);

	switch (ebp.value) {
		default:
		case svgdom::enable_background::accumulate:
			return default_value;
		case svgdom::enable_background::new_background:
			{
				std::stringstream ss;

				ss << "new";

				if (ebp.is_rect_specified()) {
					ss << " " << ebp.rect.p.x();
					ss << " " << ebp.rect.p.y();
					ss << " " << ebp.rect.d.x();
					ss << " " << ebp.rect.d.y();
				}

				return ss.str();
			}
	}
}

namespace {
uint32_t hsl_to_rgb(real h, real s, real l)
{
	using std::abs;
	using std::fmod;
	real c = (real(1) - (abs(real(2) * l - real(1)))) * s;
	real x = c * (1 - (abs(fmod(h / (real(utki::pi_degrees) / 3), real(2)) - 1)));

	real m = l - c / real(2);

	real r = 1;
	real g = 1;
	real b = 1;
	if (h < real(utki::pi_degrees) / 3) {
		r = c;
		g = x;
		b = 0;
	} else if (h < 2 * real(utki::pi_degrees) / 3) {
		r = x;
		g = c;
		b = 0;
	} else if (h < real(utki::pi_degrees)) {
		r = 0;
		g = c;
		b = x;
	} else if (h < real(utki::pi_degrees) + real(utki::pi_degrees) / 3) {
		r = 0;
		g = x;
		b = c;
	} else if (h < real(utki::pi_degrees) + 2 * real(utki::pi_degrees) / 3) {
		r = x;
		g = 0;
		b = c;
	} else if (h < 2 * real(utki::pi_degrees)) {
		r = c;
		g = 0;
		b = x;
	}

	r += m;
	g += m;
	b += m;
	r *= utki::byte_mask;
	g *= utki::byte_mask;
	b *= utki::byte_mask;

	uint32_t ret = 0;

	ret |= uint32_t(r);
	ret |= (uint32_t(g) << utki::byte_bits);
	ret |= (uint32_t(b) << (utki::byte_bits * 2));

	return ret;
}
} // namespace

// 'str' should have no leading and/or trailing white spaces.
style_value svgdom::parse_paint(std::string_view str)
{
	// TRACE(<< "parse_paint(): str = " << str << std::endl)
	if (str.empty()) {
		return {style_value_special::none};
	}

	ASSERT(!utki::string_parser::is_space(str[0])) // leading spaces should be skept already

	{
		auto ret = parse_url(str);
		if (is_valid(ret)) {
			// TRACE(<< "parsed as URL: " << std::get<std::string>(ret) << std::endl)
			return ret;
		}
	}

	if (str == none_word) {
		return {style_value_special::none};
	}

	if (str == current_color_word) {
		return {style_value_special::current_color};
	}

	// check if #-notation
	if (str[0] == '#') {
		constexpr auto num_digits_three = 3;
		constexpr auto num_digits_six = 6;

		utki::string_parser p(str.substr(1, num_digits_six));

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
		std::array<uint8_t, num_digits_six> d;
		unsigned num_digits = 0;
		for (auto i = d.begin(); i != d.end() && !p.empty(); ++i, ++num_digits) {
			char c = p.read_char();
			if ('0' <= c && c <= '9') {
				(*i) = c - '0';
			} else if ('a' <= c && c <= 'f') {
				(*i) = utki::to_int(utki::integer_base::dec) + (c - 'a');
			} else if ('A' <= c && c <= 'F') {
				(*i) = utki::to_int(utki::integer_base::dec) + (c - 'A');
			} else {
				break;
			}

			ASSERT(((*i) & utki::upper_nibble_mask) == 0) // only one hex digit
		}

		switch (num_digits) {
			case num_digits_three:
				{
					uint32_t color = 0;
					auto shift = 0;

					auto end = std::next(d.begin(), 3);
					ASSERT(d.size() >= 3)
					ASSERT(end <= d.end())

					for (auto i = d.begin(); i != end; ++i) {
						color |= (((uint32_t(*i) << utki::nibble_bits)) << shift);
						color |= ((uint32_t(*i) & utki::lower_nibble_mask) << shift);
						shift += utki::byte_bits;
					}

					return {color};
				}
			case num_digits_six:
				{
					uint32_t color = 0;
					auto shift = 0;

					for (auto i = d.begin(); i != d.end(); ++i) {
						color |= (((uint32_t(*i) << utki::nibble_bits)) << shift);
						++i;
						ASSERT(i != d.end())
						color |= ((uint32_t(*i) & utki::lower_nibble_mask) << shift);
						shift += utki::byte_bits;
					}

					return {color};
				}
			default:
				return {style_value_special::none};
		}
	}

	// check if rgb() or RGB() notation
	{
		const static std::string rgb_word = "rgb("s;

		utki::string_parser p(str);

		if (rgb_word == p.read_chars(rgb_word.size())) {
			switch (utki::string_parser(p.get_view()).skip_inclusive_until_one_of(",%")) {
				// rgb() color values are given as percentages
				case '%':
					{
						p.skip_whitespaces();
						auto r = p.read_number<float>();
						p.skip_whitespaces();
						if (p.read_char() != '%') {
							ASSERT(false)
							break;
						}
						p.skip_whitespaces_and_comma();
						auto g = p.read_number<float>();
						p.skip_whitespaces();
						if (p.read_char() != '%') {
							ASSERT(false)
							break;
						}
						p.skip_whitespaces_and_comma();
						auto b = p.read_number<float>();
						p.skip_whitespaces();
						if (p.read_char() != '%') {
							ASSERT(false, [&](auto& o) {
								o << "str = " << str;
							})
							break;
						}
						p.skip_whitespaces();

						if (p.read_char() != ')') {
							// no expected closing ')'
							break;
						}

						// std::cout << "r = " << r << ", g = " << g << ", b = " << b << std::endl;
						auto color = uint32_t(r * utki::byte_mask / std::centi::den) |
							(uint32_t(g * utki::byte_mask / std::centi::den) << utki::byte_bits) |
							(uint32_t(b * utki::byte_mask / std::centi::den) << (utki::byte_bits * 2));
						return {color};
					}
				// rgb() color values are given as absolute values
				case ',':
					{
						p.skip_whitespaces();
						auto r = p.read_number<uint32_t>();
						p.skip_whitespaces_and_comma();
						auto g = p.read_number<uint32_t>();
						p.skip_whitespaces_and_comma();
						auto b = p.read_number<uint32_t>();
						p.skip_whitespaces();

						if (p.read_char() != ')') {
							// no expected closing ')'
							break;
						}

						auto color = r | (g << utki::byte_bits) | (b << (utki::byte_bits * 2));
						return {color};
					}
				// malformed rgb() paint specification
				default:
					ASSERT(false)
					break;
			}
			return {style_value_special::none};
		}
	}

	// check if hsl() notation
	{
		const static std::string hsl_word = "hsl("s;

		utki::string_parser p(str);

		if (hsl_word == p.read_chars(hsl_word.size())) {
			p.skip_whitespaces();
			auto h = p.read_number<uint32_t>();
			p.skip_whitespaces_and_comma();
			auto s = p.read_number<uint32_t>();
			if (p.read_char() != '%') {
				return {style_value_special::none};
			}
			p.skip_whitespaces_and_comma();
			auto l = p.read_number<uint32_t>();
			if (p.read_char() != '%') {
				return {style_value_special::none};
			}
			p.skip_whitespaces();

			if (p.read_char() == ')') {
				auto color = hsl_to_rgb( //
					real(h),
					real(s) / real(std::centi::den),
					real(l) / real(std::centi::den)
				);
				return {color};
			}
			return {style_value_special::none};
		}
	}

	// check if color name
	{
		utki::string_parser p(str);
		auto name = p.read_word();

		auto i = color_name_to_color_map.find(name);
		if (i != color_name_to_color_map.end()) {
			ASSERT(i->first == name)
			return {i->second};
		}
	}

	return {style_value_special::none};
}

std::string svgdom::paint_to_string(const style_value& v)
{
	if (std::holds_alternative<style_value_special>(v)) { // special value
		switch (*std::get_if<style_value_special>(&v)) {
			case style_value_special::none:
				return none_word;
			case style_value_special::inherit: // TODO: isn't it already handled in styleable::style_value_to_string()?
				return inherit_word;
			case style_value_special::current_color:
				return current_color_word;
			default:
				return {};
		}
	} else if (std::holds_alternative<std::string>(v)) { // URL
		std::stringstream ss;
		ss << "url(" << *std::get_if<std::string>(&v) << ")";
		return ss.str();
	} else if (std::holds_alternative<uint32_t>(v)) {
		auto i = color_to_color_name_map.find(*std::get_if<uint32_t>(&v));
		if (i != color_to_color_name_map.end()) {
			// color name

			return std::string(i->second);
		} else {
			// #-notation

			std::stringstream s;
			s << std::hex;
			s << "#";

			uint32_t val = std::get<uint32_t>(v);

			for (auto i = 0; i != 3; ++i) {
				s << ((val >> utki::nibble_bits) & utki::lower_nibble_mask);
				s << (val & utki::lower_nibble_mask);
				val >>= utki::byte_bits;
			}

			return s.str();
		}
	}
	return {};
}

std::string svgdom::get_local_id_from_iri(const style_value& v)
{
	if (!std::holds_alternative<std::string>(v)) {
		return {};
	}
	return iri_to_local_id(*std::get_if<std::string>(&v));
}
