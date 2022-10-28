/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

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

#include <map>
#include <vector>
#include <variant>

#include <cssom/om.hpp>

#include <r4/vector.hpp>
#include <r4/rectangle.hpp>

#include "../config.hpp"
#include "../length.hpp"

namespace svgdom{

/**
 * @brief Style properties enumeration.
 */
enum class style_property : uint32_t{
	unknown,					// 0
	font,
	font_family,
	font_size,
	font_size_adjust,
	font_stretch,				// 5
	font_style,
	font_variant,
	font_weight,
	direction,
	letter_spacing,				// 10
	text_decoration,
	unicode_bidi,
	word_spacing,
	clip,
	color,						// 15
	cursor,
	display,
	overflow,
	visibility,
	clip_path,					// 20
	clip_rule,
	mask,
	opacity,
	enable_background,
	filter,						// 25
	flood_color,
	flood_opacity,
	lighting_color,
	stop_color,
	stop_opacity,				// 30
	pointer_events,
	color_interpolation,
	color_interpolation_filters,
	color_profile,
	color_rendering,			// 35
	fill,
	fill_opacity,
	fill_rule,
	image_rendering,
	marker,						// 40
	marker_end,
	marker_mid,
	marker_start,
	shape_rendering,
	stroke,						// 45
	stroke_dasharray,
	stroke_dashoffset,
	stroke_linecap,
	stroke_linejoin,
	stroke_miterlimit,			// 50
	stroke_opacity,
	stroke_width,
	text_rendering,
	alignment_baseline,
	baseline_shift,				// 55
	dominant_baseline,
	glyph_orientation_horizontal,
	glyph_orientation_vertical,
	kerning,
	text_anchor,				// 60
	writing_mode,
	
	enum_size
};

enum class stroke_line_cap{
	butt,
	round,
	square
};

enum class stroke_line_join{
	miter,
	round,
	bevel
};

enum class fill_rule{
	nonzero,
	evenodd
};

/**
 * @brief Enumeration of color-interpolation-filters style property values.
 */
enum class color_interpolation{
	auto_,
	s_rgb,
	linear_rgb
};

enum class display{
	inline_,
	block,
	list_item,
	run_in,
	compact,
	marker,
	table,
	inline_table,
	table_row_group,
	table_header_group,
	table_footer_group,
	table_row,
	table_column_group,
	table_column,
	table_cell,
	table_caption,
	none
};

enum class visibility{
	visible,
	hidden,
	collapse
};

enum class enable_background{
	accumulate,
	new_
};

struct enable_background_property{
	enable_background value;
	r4::rectangle<real> rect; // this only make sense when 'value' is NEW.
	
	bool is_rect_specified()const noexcept{
		return this->rect.d.x() >= 0 && this->rect.d.y() >= 0;
	}
};

enum class style_value_special{
	unknown,
	none,
	current_color,
	inherit
};

typedef std::variant<
		style_value_special,
		uint32_t,
		real,
		length,
		svgdom::stroke_line_cap,
		svgdom::stroke_line_join,
		svgdom::fill_rule,
		svgdom::color_interpolation,
		svgdom::display,
		svgdom::enable_background_property,
		svgdom::visibility,
		std::string,
		std::vector<length>
	> style_value;

inline bool is_valid(const style_value& v){
	return !std::holds_alternative<style_value_special>(v) || *std::get_if<style_value_special>(&v) != style_value_special::unknown;
}

inline bool is_none(const style_value& v){
	return std::holds_alternative<style_value_special>(v) && *std::get_if<style_value_special>(&v) == style_value_special::none;
}

inline bool is_inherit(const style_value& v){
	return std::holds_alternative<style_value_special>(v) && *std::get_if<style_value_special>(&v) == style_value_special::inherit;
}

inline bool is_current_color(const style_value& v){
	return std::holds_alternative<style_value_special>(v) && *std::get_if<style_value_special>(&v) == style_value_special::current_color;
}

/**
 * @brief Get ID of the locally referenced element.
 * If the value represents an URL this function will return local ID of the referenced element.
 * @param v - stly value holding an URL.
 * @return ID of a locally referenced element.
 * @return Empty string if the value is not URL or the reference is not local IRI.
 */
std::string get_local_id_from_iri(const style_value& v);

style_value parse_paint(std::string_view str);
std::string paint_to_string(const style_value& v);

style_value parse_color_interpolation(std::string_view str);
	
style_value parse_display(std::string_view& str);
std::string_view display_to_string(const style_value& v);

style_value parse_visibility(std::string_view str);
std::string_view visibility_to_string(const style_value& v);
	
style_value parse_enable_background(std::string_view str);
std::string enable_background_to_string(const style_value& v);
	
std::string color_interpolation_filters_to_string(const style_value& v);

style_value parse_url(std::string_view str);

/**
 * @brief get color as RGB.
 * If style value represents a color then this function returns the
 * color as red, green and blue values.
 * @param v - style value to get RGB values from.
 * @return RGB structure holding red, green and blue.
 */
r4::vector3<real> get_rgb(const style_value& v);
	
/**
 * @brief Make style_value representing a color.
 * @param r - red component, from [0, 0xff].
 * @param g - green component, from [0, 0xff].
 * @param b - blue component, from [0, 0xff].
 */
style_value make_style_value(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Make style_value representing a color.
 * @param rgb - red, green, blue color components, each from [0, 1].
 */
style_value make_style_value(const r4::vector3<real>& rgb);


/**
 * @brief An element which has 'style' attribute or can be styled.
 */
struct styleable : public cssom::styleable{
	std::map<style_property, style_value> styles;
	std::map<style_property, style_value> presentation_attributes;

	std::vector<std::string> classes;

	utki::span<const std::string> get_classes()const override{
		return utki::make_span(this->classes);
	}

	virtual ~styleable()noexcept{}

	const style_value* get_style_property(style_property p)const;

	const style_value* get_presentation_attribute(style_property p)const;

	std::string classes_to_string()const;

	std::string styles_to_string()const;

	static std::string style_value_to_string(style_property p, const style_value& v);

	static decltype(styles) parse(const std::string& str);

	static style_value parse_style_property_value(style_property type, std::string_view str);

	static bool is_inherited(style_property p);

	static std::string_view property_to_string(style_property p);
	static style_property string_to_property(std::string_view str);
};

}
