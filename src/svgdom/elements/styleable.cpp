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

#include "styleable.hpp"

#include <set>
#include <iomanip>
#include <cctype>
#include <array>
#include <cmath>

#include <utki/debug.hpp>
#include <utki/util.hpp>

#include "../util.hxx"
#include "element.hpp"
#include "../malformed_svg_error.hpp"

using namespace svgdom;

namespace{
const char* none_word = "none";
const char* inherit_word = "inherit";
const char* current_color_word = "currentColor";
}

namespace{
std::string stroke_dasharray_to_string(const style_value& v){
	// special values must be already handled by styleable::style_value_to_string() at this point
	ASSERT(
		!std::holds_alternative<style_value_special>(v),
		[&](auto&o){o << "v = " << unsigned(*std::get_if<style_value_special>(&v));}
	)

	if(!std::holds_alternative<std::vector<length>>(v)){
		return std::string();
	}

	std::stringstream ss;

	auto dasharray = *std::get_if<std::vector<length>>(&v);

	for(auto i = dasharray.begin(); i != dasharray.end(); ++i){
		if(i != dasharray.begin()){
			ss << " ";
		}
		ss << (*i);
	}

	return ss.str();
}
}

namespace{
style_value parse_stroke_dasharray(std::string_view str){
	// the "inherit" case is handled already by styleable::parse_style_property_value()
	if(str == none_word){
		return style_value(style_value_special::none);
	}

	std::vector<length> dasharray;

	utki::string_parser p(str);

	p.skip_whitespaces();

	while(!p.empty()){
		auto len_str = p.read_word_until(',');
		auto l = length::parse(len_str);
		dasharray.push_back(l);

		p.skip_whitespaces_and_comma();
	}

	return style_value(std::move(dasharray));
}
}

std::string styleable::style_value_to_string(style_property p, const style_value& v){
	if(!is_valid(v)){
		return std::string();
	}

	if(is_inherit(v)){
		return inherit_word;
	}else if(is_none(v)){
		return none_word;
	}else if(is_current_color(v)){
		return current_color_word;
	}

	std::stringstream s;
	switch(p){
		default:
			LOG([&](auto&o){o << "Unimplemented style property: " << styleable::property_to_string(p) << ", writing empty value." << std::endl;})
			break;
		case style_property::color_interpolation_filters:
			s << color_interpolation_filters_to_string(v);
			break;
		case style_property::stroke_miterlimit:
		case style_property::stop_opacity:
		case style_property::opacity:
		case style_property::stroke_opacity:
		case style_property::fill_opacity:
			if(std::holds_alternative<real>(v)){
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
			if(std::holds_alternative<length>(v)){
				s << *std::get_if<length>(&v);
			}
			break;
		case style_property::stroke_linecap:
			if(std::holds_alternative<svgdom::stroke_line_cap>(v)){
				switch(*std::get_if<svgdom::stroke_line_cap>(&v)){
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
			if(std::holds_alternative<svgdom::stroke_line_join>(v)){
				switch(*std::get_if<svgdom::stroke_line_join>(&v)){
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
			if(std::holds_alternative<svgdom::fill_rule>(v)){
				switch(*std::get_if<svgdom::fill_rule>(&v)){
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
			if(std::holds_alternative<std::string>(v)){
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

std::string styleable::styles_to_string()const{
	std::stringstream s;
	
	bool isFirst = true;
	
	for(auto& st : this->styles){
		if(isFirst){
			isFirst = false;
		}else{
			s << "; ";
		}
		
		ASSERT(st.first != style_property::unknown)
		
		s << property_to_string(st.first) << ":";
		
		s << style_value_to_string(st.first, st.second);
	}
	return s.str();
}

std::string styleable::classes_to_string()const{
	std::stringstream ss;

	for(auto i = this->classes.begin(); i != this->classes.end(); ++i){
		if(i != this->classes.begin()){
			ss << " ";
		}
		ss << *i;
	}

	return ss.str();
}

// input parameter 'str' should have no leading or trailing white spaces
style_value styleable::parse_style_property_value(style_property type, std::string_view str){
	if(str == inherit_word){
		return style_value(style_value_special::inherit);
	}

	switch(type){
		default:
			LOG([&](auto&o){o << "unimplemented style property encountered: " << styleable::property_to_string(type) << std::endl;})
			return style_value(style_value_special::unknown);
		case style_property::color_interpolation_filters:
			return parse_color_interpolation(str);
		case style_property::stroke_miterlimit:
			{
				real miter_limit = utki::string_parser(str).read_number<real>();
				using std::max;
				miter_limit = max(miter_limit, real(1)); // minimal value is 1
				return style_value(miter_limit);
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
				return style_value(opacity);
			}
			break;
		case style_property::stop_color:
		case style_property::fill:
		case style_property::stroke:
			return parse_paint(str);
		case style_property::stroke_dashoffset:
		case style_property::stroke_width:
			return style_value(length::parse(str));
		case style_property::stroke_linecap:
			{
				stroke_line_cap slc;
				if(str == "butt"){
					slc = stroke_line_cap::butt;
				}else if(str == "round"){
					slc = stroke_line_cap::round;
				}else if(str == "square"){
					slc = stroke_line_cap::square;
				}else{
					LOG([&](auto&o){o << "unknown strokeLineCap value:" << str << std::endl;})
					return style_value();
				}
				return style_value(slc);
			}
		case style_property::stroke_linejoin:
			{
				stroke_line_join slj;
				if(str == "miter"){
					slj = stroke_line_join::miter;
				}else if(str == "round"){
					slj = stroke_line_join::round;
				}else if(str == "bevel"){
					slj = stroke_line_join::bevel;
				}else{
					LOG([&](auto&o){o << "unknown strokeLineJoin value:" << str << std::endl;})
					return style_value();
				}
				return style_value(slj);
			}
		case style_property::fill_rule:
			{
				fill_rule fr;
				if(str == "nonzero"){
					fr = fill_rule::nonzero;
				}else if(str == "evenodd"){
					fr = fill_rule::evenodd;
				}else{
					LOG([&](auto&o){o << "unknown fill-rule value:" << str << std::endl;})
					return style_value();
				}
				return style_value(fr);
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

style_value svgdom::parse_url(std::string_view str){
	const static std::string url_word = "url(";
	
	utki::string_parser p(str);
	p.skip_whitespaces();
	
	if(url_word != p.read_chars(url_word.size())){
		return style_value(style_value_special::unknown);
	}

	p.skip_whitespaces();
	auto url = p.read_word_until(')');

	p.skip_whitespaces();
	if(p.read_char() == ')'){
		return style_value(std::string(url));
	}

	return style_value(style_value_special::unknown);;
}

namespace{
style_value parse_style_property_value(style_property type, utki::string_parser& p){
	p.skip_whitespaces();
	auto str = p.read_chars_until(';');
	return styleable::parse_style_property_value(type, trim_tail(str));
}
}

decltype(styleable::styles) styleable::parse(const std::string& str){
	utki::string_parser p(str);
	
	p.skip_whitespaces();

	decltype(styleable::styles) ret;
	
	while(!p.empty()){
		auto property = p.read_word_until(':');
		
		style_property type = styleable::string_to_property(property);
		
		if(type == style_property::unknown){
			// unknown style property, skip it
			LOG([&](auto&o){o << "Unknown style property: " << property << std::endl;})
			LOG([&](auto&o){o << "str = " << str << std::endl;})
			LOG([&](auto&o){o << "ret.size() = " << ret.size() << std::endl;})
			p.skip_inclusive_until(';');
			continue;
		}
		
		p.skip_whitespaces();
		
		if(p.read_char() != ':'){
			return ret; // expected colon
		}
		
		style_value v = ::parse_style_property_value(type, p);
		
		p.skip_whitespaces();
		
		if(!p.empty() && p.read_char() != ';'){
			return ret; // expected semicolon
		}
		
		ret[type] = std::move(v);
		
		p.skip_whitespaces();
	}
	
	return ret;
}

namespace{
const std::set<style_property> nonInheritedStyleProperties = {
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
}

bool styleable::is_inherited(style_property p) {
	return nonInheritedStyleProperties.find(p) == nonInheritedStyleProperties.end();
}

namespace{
const std::map<std::string_view, style_property> string_to_property_map = {
	{"alignment-baseline", style_property::alignment_baseline},
	{"baseline-shift", style_property::baseline_shift},
	{"clip", style_property::clip},
	{"clip-path", style_property::clip_path},
	{"clip-rule", style_property::clip_rule},
	{"color", style_property::color},
	{"color-interpolation", style_property::color_interpolation},
	{"color-interpolation-filters", style_property::color_interpolation_filters},
	{"color-profile", style_property::color_profile},
	{"color-rendering", style_property::color_rendering},
	{"cursor", style_property::cursor},
	{"direction", style_property::direction},
	{"display", style_property::display},
	{"dominant-baseline", style_property::dominant_baseline},
	{"enable-background", style_property::enable_background},
	{"fill", style_property::fill},
	{"fill-opacity", style_property::fill_opacity},
	{"fill-rule", style_property::fill_rule},
	{"filter", style_property::filter},
	{"flood-color", style_property::flood_color},
	{"flood-opacity", style_property::flood_opacity},
	{"font", style_property::font},
	{"font-family", style_property::font_family},
	{"font-size", style_property::font_size},
	{"font-size-adjust", style_property::font_size_adjust},
	{"font-stretch", style_property::font_stretch},
	{"font-style", style_property::font_style},
	{"font-variant", style_property::font_variant},
	{"font-weight", style_property::font_weight},
	{"glyph-orientation-horizontal", style_property::glyph_orientation_horizontal},
	{"glyph-orientation-vertical", style_property::glyph_orientation_vertical},
	{"image-rendering", style_property::image_rendering},
	{"kerning", style_property::kerning},
	{"letter-spacing", style_property::letter_spacing},
	{"lighting-color", style_property::lighting_color},
	{"marker", style_property::marker},
	{"marker-end", style_property::marker_end},
	{"marker-mid", style_property::marker_mid},
	{"marker-start", style_property::marker_start},
	{"mask", style_property::mask},
	{"opacity", style_property::opacity},
	{"overflow", style_property::overflow},
	{"pointer-events", style_property::pointer_events},
	{"shape-rendering", style_property::shape_rendering},
	{"stop-color", style_property::stop_color},
	{"stop-opacity", style_property::stop_opacity},
	{"stroke", style_property::stroke},
	{"stroke-dasharray", style_property::stroke_dasharray},
	{"stroke-dashoffset", style_property::stroke_dashoffset},
	{"stroke-linecap", style_property::stroke_linecap},
	{"stroke-linejoin", style_property::stroke_linejoin},
	{"stroke-miterlimit", style_property::stroke_miterlimit},
	{"stroke-opacity", style_property::stroke_opacity},
	{"stroke-width", style_property::stroke_width},
	{"text-anchor", style_property::text_anchor},
	{"text-decoration", style_property::text_decoration},
	{"text-rendering", style_property::text_rendering},
	{"unicode-bidi", style_property::unicode_bidi},
	{"visibility", style_property::visibility},
	{"word-spacing", style_property::word_spacing},
	{"writing-mode", style_property::writing_mode}
};
}

namespace{
auto property_to_string_map = utki::flip_map(string_to_property_map);
}

style_property styleable::string_to_property(std::string_view str){
	auto i = string_to_property_map.find(str);
	if(i != string_to_property_map.end()){
		return i->second;
	}
	
	return style_property::unknown;
}

std::string_view styleable::property_to_string(style_property p){
	auto i = property_to_string_map.find(p);
	if(i != property_to_string_map.end()){
		return i->second;
	}
	return std::string_view();
}

const style_value* styleable::get_style_property(style_property p)const{
	auto i = this->styles.find(p);
	if(i != this->styles.end()){
		return &i->second;
	}
	return nullptr;
}

const style_value* styleable::get_presentation_attribute(style_property p)const{
	auto i = this->presentation_attributes.find(p);
	if(i != this->presentation_attributes.end()){
		return &i->second;
	}
	return nullptr;
}

r4::vector3<real> svgdom::get_rgb(const style_value& v){
	if(!std::holds_alternative<uint32_t>(v)){
		return 0;
	}

	auto c = *std::get_if<uint32_t>(&v);
	
	r4::vector3<real> ret;
	
	ret.r() = real(c & 0xff) / real(0xff);
	ret.g() = real((c >> 8) & 0xff) / real(0xff);
	ret.b() = real((c >> 16) & 0xff) / real(0xff);
	
	return ret;
}

style_value svgdom::make_style_value(uint8_t r, uint8_t g, uint8_t b){
	return style_value(uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16));
}

style_value svgdom::make_style_value(const r4::vector3<real>& rgb){
	auto c = (rgb * 0xff).to<uint8_t>();
	return make_style_value(
			uint8_t(c.r()),
			uint8_t(c.g()),
			uint8_t(c.b())
		);
}

namespace{
const std::map<std::string_view, uint32_t> color_name_to_color_map = {
	{"aliceblue", 0xfff8f0},
	{"antiquewhite", 0xd7ebfa},
	{"aqua", 0xffff00},
	{"aquamarine", 0xd4ff7f},
	{"azure", 0xfffff0},
	{"beige", 0xdcf5f5},
	{"bisque", 0xc4e4ff},
	{"black", 0x0},
	{"blanchedalmond", 0xcdebff},
	{"blue", 0xff0000},
	{"blueviolet", 0xe22b8a},
	{"brown", 0x2a2aa5},
	{"burlywood", 0x87b8de},
	{"cadetblue", 0xa09e5f},
	{"chartreuse", 0xff7f},
	{"chocolate", 0x1e69d2},
	{"coral", 0x507fff},
	{"cornflowerblue", 0xed9564},
	{"cornsilk", 0xdcf8ff},
	{"crimson", 0x3c14dc},
	{"cyan", 0xffff00},
	{"darkblue", 0x8b0000},
	{"darkcyan", 0x8b8b00},
	{"darkgoldenrod", 0xb86b8},
	{"darkgray", 0xa9a9a9},
	{"darkgreen", 0x6400},
	{"darkgrey", 0xa9a9a9},
	{"darkkhaki", 0x6bb7bd},
	{"darkmagenta", 0x8b008b},
	{"darkolivegreen", 0x2f6b55},
	{"darkorange", 0x8cff},
	{"darkorchid", 0xcc3299},
	{"darkred", 0x8b},
	{"darksalmon", 0x7a96e9},
	{"darkseagreen", 0x8fbc8f},
	{"darkslateblue", 0x8b3d48},
	{"darkslategray", 0x4f4f2f},
	{"darkslategrey", 0x4f4f2f},
	{"darkturquoise", 0xd1ce00},
	{"darkviolet", 0xd30094},
	{"deeppink", 0x9314ff},
	{"deepskyblue", 0xffbf00},
	{"dimgray", 0x696969},
	{"dimgrey", 0x696969},
	{"dodgerblue", 0xff901e},
	{"firebrick", 0x2222b2},
	{"floralwhite", 0xf0faff},
	{"forestgreen", 0x228b22},
	{"fuchsia", 0xff00ff},
	{"gainsboro", 0xdcdcdc},
	{"ghostwhite", 0xfff8f8},
	{"gold", 0xd7ff},
	{"goldenrod", 0x20a5da},
	{"gray", 0x808080},
	{"grey", 0x808080},
	{"green", 0x8000},
	{"greenyellow", 0x2fffad},
	{"honeydew", 0xf0fff0},
	{"hotpink", 0xb469ff},
	{"indianred", 0x5c5ccd},
	{"indigo", 0x82004b},
	{"ivory", 0xf0ffff},
	{"khaki", 0x8ce6f0},
	{"lavender", 0xfae6e6},
	{"lavenderblush", 0xf5f0ff},
	{"lawngreen", 0xfc7c},
	{"lemonchiffon", 0xcdfaff},
	{"lightblue", 0xe6d8ad},
	{"lightcoral", 0x8080f0},
	{"lightcyan", 0xffffe0},
	{"lightgoldenrodyellow", 0xd2fafa},
	{"lightgray", 0xd3d3d3},
	{"lightgreen", 0x90ee90},
	{"lightgrey", 0xd3d3d3},
	{"lightpink", 0xc1b6ff},
	{"lightsalmon", 0x7aa0ff},
	{"lightseagreen", 0xaab220},
	{"lightskyblue", 0xface87},
	{"lightslategray", 0x998877},
	{"lightslategrey", 0x998877},
	{"lightsteelblue", 0xdec4b0},
	{"lightyellow", 0xe0ffff},
	{"lime", 0xff00},
	{"limegreen", 0x32cd32},
	{"linen", 0xe6f0fa},
	{"magenta", 0xff00ff},
	{"maroon", 0x80},
	{"mediumaquamarine", 0xaacd66},
	{"mediumblue", 0xcd0000},
	{"mediumorchid", 0xd355ba},
	{"mediumpurple", 0xdb7093},
	{"mediumseagreen", 0x71b33c},
	{"mediumslateblue", 0xee687b},
	{"mediumspringgreen", 0x9afa00},
	{"mediumturquoise", 0xccd148},
	{"mediumvioletred", 0x8515c7},
	{"midnightblue", 0x701919},
	{"mintcream", 0xfafff5},
	{"mistyrose", 0xe1e4ff},
	{"moccasin", 0xb5e4ff},
	{"navajowhite", 0xaddeff},
	{"navy", 0x800000},
	{"oldlace", 0xe6f5fd},
	{"olive", 0x8080},
	{"olivedrab", 0x238e6b},
	{"orange", 0xa5ff},
	{"orangered", 0x45ff},
	{"orchid", 0xd670da},
	{"palegoldenrod", 0xaae8ee},
	{"palegreen", 0x98fb98},
	{"paleturquoise", 0xeeeeaf},
	{"palevioletred", 0x9370db},
	{"papayawhip", 0xd5efff},
	{"peachpuff", 0xb9daff},
	{"peru", 0x3f85cd},
	{"pink", 0xcbc0ff},
	{"plum", 0xdda0dd},
	{"powderblue", 0xe6e0b0},
	{"purple", 0x800080},
	{"red", 0xff},
	{"rosybrown", 0x8f8fbc},
	{"royalblue", 0xe16941},
	{"saddlebrown", 0x13458b},
	{"salmon", 0x7280fa},
	{"sandybrown", 0x60a4f4},
	{"seagreen", 0x578b2e},
	{"seashell", 0xeef5ff},
	{"sienna", 0x2d52a0},
	{"silver", 0xc0c0c0},
	{"skyblue", 0xebce87},
	{"slateblue", 0xcd5a6a},
	{"slategray", 0x908070},
	{"slategrey", 0x908070},
	{"snow", 0xfafaff},
	{"springgreen", 0x7fff00},
	{"steelblue", 0xb48246},
	{"tan", 0x8cb4d2},
	{"teal", 0x808000},
	{"thistle", 0xd8bfd8},
	{"tomato", 0x4763ff},
	{"turquoise", 0xd0e040},
	{"violet", 0xee82ee},
	{"wheat", 0xb3def5},
	{"white", 0xffffff},
	{"whitesmoke", 0xf5f5f5},
	{"yellow", 0xffff},
	{"yellowgreen", 0x32cd9a}
};
}

namespace{
const auto color_to_color_name_map = utki::flip_map(color_name_to_color_map);
}

namespace{
std::map<std::string_view, display> string_to_display_map = {
	{"inline", svgdom::display::inline_},
	{"block", svgdom::display::block},
	{"list-item", svgdom::display::list_item},
	{"run-in", svgdom::display::run_in},
	{"compact", svgdom::display::compact},
	{"marker", svgdom::display::marker},
	{"table", svgdom::display::table},
	{"inline-table", svgdom::display::inline_table},
	{"table-row-group", svgdom::display::table_row_group},
	{"table-header-group", svgdom::display::table_header_group},
	{"table-footer-group", svgdom::display::table_footer_group},
	{"table-row", svgdom::display::table_row},
	{"table-column-group", svgdom::display::table_column_group},
	{"table-column", svgdom::display::table_column},
	{"table-cell", svgdom::display::table_cell},
	{"table-caption", svgdom::display::table_caption},
	{none_word, svgdom::display::none}
};
}

namespace{
auto display_to_string_map = utki::flip_map(string_to_display_map);
}

style_value svgdom::parse_display(std::string_view& str){
	// NOTE: "inherit" is already checked on upper level.

	auto i = string_to_display_map.find(str);
	if(i == string_to_display_map.end()){
		return style_value(svgdom::display::inline_); // default value
	}else{
		return style_value(i->second);
	}
}

std::string_view svgdom::display_to_string(const style_value& v){
	const auto& default_value = display_to_string_map[svgdom::display::inline_];

	if(!std::holds_alternative<svgdom::display>(v)){
		return default_value;
	}

	auto i = display_to_string_map.find(*std::get_if<svgdom::display>(&v));
	if(i == display_to_string_map.end()){
		return default_value;
	}
	return i->second;
}

namespace{
std::map<std::string_view, visibility> string_to_visibility_map = {
	{"visible", visibility::visible},
	{"hidden", visibility::hidden},
	{"collapse", visibility::collapse}
};
}

namespace{
auto visibility_to_string_map = utki::flip_map(string_to_visibility_map);
}

style_value svgdom::parse_visibility(std::string_view str){
	// NOTE: "inherit" is already checked on upper level.
	
	auto i = string_to_visibility_map.find(str);
	if(i == string_to_visibility_map.end()){
		return style_value(svgdom::visibility::visible); // default value
	}else{
		return style_value(i->second);
	}
}

std::string_view svgdom::visibility_to_string(const style_value& v){
	const auto& default_value = visibility_to_string_map[svgdom::visibility::visible];

	if(!std::holds_alternative<svgdom::visibility>(v)){
		return default_value;
	}

	auto i = visibility_to_string_map.find(*std::get_if<svgdom::visibility>(&v));
	if(i == visibility_to_string_map.end()){
		return default_value;
	}
	return i->second;
}

style_value svgdom::parse_color_interpolation(std::string_view str){
	color_interpolation v;
	if(str == "auto"){
		v = color_interpolation::auto_;
	}else if(str == "linearRGB"){
		v = color_interpolation::linear_rgb;
	}else if(str == "sRGB"){
		v = color_interpolation::s_rgb;
	}else{
		return style_value(style_value_special::unknown);
	}
	
	return style_value(v);
}

namespace{
std::string to_string(color_interpolation ci){
	switch(ci){
		case color_interpolation::auto_:
			return "auto";
		case color_interpolation::linear_rgb:
			return "linearRGB";
		case color_interpolation::s_rgb:
			return "sRGB";
		default:
			ASSERT(false, [&](auto&o){o << "ci = " << unsigned(ci);})
			return "";
	}
}
}

std::string svgdom::color_interpolation_filters_to_string(const style_value& v){
	if(!std::holds_alternative<svgdom::color_interpolation>(v)){
		return std::string();
	}

	return to_string(*std::get_if<svgdom::color_interpolation>(&v));
}

namespace{
enable_background_property parse_enable_background_new_rect(std::string_view str){
	enable_background_property ret;
	
	try{
		utki::string_parser p(str);
		p.skip_inclusive_until(' '); // skip 'new'

		p.skip_whitespaces();
		
		if(p.empty()){
			ret.rect.d.x() = -1; // indicate that rectangle is not specified
			return ret;
		}
		
		ret.rect.p.x() = p.read_number<real>();
		ret.rect.p.y() = p.read_number<real>();
		ret.rect.d.x() = p.read_number<real>();
		ret.rect.d.y() = p.read_number<real>();
	}catch(std::invalid_argument&){
		throw malformed_svg_error("malformed enable-background NEW string");
	}
	
	return ret;
}
}

style_value svgdom::parse_enable_background(std::string_view str){
	enable_background_property ebp;

	std::string newStr = "new";
	if(str.substr(0, newStr.length()) == "new"){
		try{
			ebp = parse_enable_background_new_rect(str);
			ebp.value = svgdom::enable_background::new_;
		}catch(malformed_svg_error&){
			ebp.value = svgdom::enable_background::accumulate; // default value
		}
	}else{
		ebp.value = svgdom::enable_background::accumulate; // default value
	}
	
	return style_value(ebp);
}

std::string svgdom::enable_background_to_string(const style_value& v){
	const std::string& default_value = "accumulate";

	if(!std::holds_alternative<svgdom::enable_background_property>(v)){
		return default_value;
	}

	auto ebp = *std::get_if<svgdom::enable_background_property>(&v);

	switch(ebp.value){
		default:
		case svgdom::enable_background::accumulate:
			return default_value;
		case svgdom::enable_background::new_:
			{
				std::stringstream ss;
				
				ss << "new";
				
				if(ebp.is_rect_specified()){
					ss << " " << ebp.rect.p.x();
					ss << " " << ebp.rect.p.y();
					ss << " " << ebp.rect.d.x();
					ss << " " << ebp.rect.d.y();
				}
				
				return ss.str();
			}
	}
}

namespace{
uint32_t hsl_to_rgb(real h, real s, real l){
	using std::abs;
	using std::fmod;
	real c = (real(1) - (abs(real(2) * l - real(1))) ) * s;
	real x = c * (1 - (abs(fmod(h / real(60), real(2)) - 1)));
	
	real m = l - c / real(2);
	
	real r = 1, g = 1, b = 1;
	if(h < real(60)){
		r = c;
		g = x;
		b = 0;
	}else if(h < real(120)){
		r = x;
		g = c;
		b = 0;
	}else if(h < real(180)){
		r = 0;
		g = c;
		b = x;
	}else if(h < real(240)){
		r = 0;
		g = x;
		b = c;
	}else if(h < real(300)){
		r = x;
		g = 0;
		b = c;
	}else if(h < real(360)){
		r = c;
		g = 0;
		b = x;
	}
	
	r += m;
	g += m;
	b += m;
	r *= 0xff;
	g *= 0xff;
	b *= 0xff;
	
	uint32_t ret = 0;
	
	ret |= uint32_t(r);
	ret |= (uint32_t(g) << 8);
	ret |= (uint32_t(b) << 16);
	
	return ret;
}
}

// 'str' should have no leading and/or trailing white spaces.
style_value svgdom::parse_paint(std::string_view str){
	// TRACE(<< "parse_paint(): str = " << str << std::endl)
	if(str.empty()){
		return style_value(style_value_special::none);
	}
	
	ASSERT(!utki::string_parser::is_space(str[0])) // leading spaces should be skept already	
	
	{
		auto ret = parse_url(str);
		if(is_valid(ret)){
			// TRACE(<< "parsed as URL: " << std::get<std::string>(ret) << std::endl)
			return ret;
		}
	}
	
	if(str == none_word){
		return style_value(style_value_special::none);
	}
	
	if(str == current_color_word){
		return style_value(style_value_special::current_color);
	}
	
	// check if #-notation
	if(str[0] == '#'){
		utki::string_parser p(str.substr(1, 6));
		
		std::array<uint8_t, 6> d;
		unsigned num_digits = 0;
		for(auto i = d.begin(); i != d.end() && !p.empty(); ++i, ++num_digits){
			char c = p.read_char();
			if('0' <= c && c <= '9'){
				(*i) = c - '0';
			}else if('a' <= c && c <= 'f'){
				(*i) = 10 + c - 'a';
			}else if('A' <= c && c <= 'F'){
				(*i) = 10 + c - 'A';
			}else{
				break;
			}
			
			ASSERT(((*i) & 0xf0) == 0) // only one hex digit
		}
		switch(num_digits){
			case 3:
				{
					auto color = (uint32_t(d[0]) << 4) | (uint32_t(d[0]))
							| (uint32_t(d[1]) << 12) | (uint32_t(d[1]) << 8)
							| (uint32_t(d[2]) << 20) | (uint32_t(d[2]) << 16);

					return style_value(color);
				}
			case 6:
				{
					auto color = (uint32_t(d[0]) << 4) | (uint32_t(d[1]))
							| (uint32_t(d[2]) << 12) | (uint32_t(d[3]) << 8)
							| (uint32_t(d[4]) << 20) | (uint32_t(d[5]) << 16);
					
					return style_value(color);
				}
			default:
				return style_value(style_value_special::none);
		}
	}
	
	// check if rgb() or RGB() notation
	{
		const static std::string rgb_word = "rgb(";

		utki::string_parser p(str);

		if(rgb_word == p.read_chars(rgb_word.size())){
			uint32_t r, g, b;
			
			p.skip_whitespaces();
			r = p.read_number<uint32_t>();
			p.skip_whitespaces_and_comma();
			g = p.read_number<uint32_t>();
			p.skip_whitespaces_and_comma();
			b = p.read_number<uint32_t>();
			p.skip_whitespaces();
			
			if(p.read_char() == ')'){
				auto color = r | (g << 8) | (b << 16);
				return style_value(color);
			}
			return style_value(style_value_special::none);
		}
	}
	
	// check if hsl() notation
	{
		const static std::string hsl_word = "hsl(";

		utki::string_parser p(str);

		if(hsl_word == p.read_chars(hsl_word.size())){
			uint32_t h, s, l;
			
			p.skip_whitespaces();
			h = p.read_number<decltype(h)>();
			p.skip_whitespaces_and_comma();
			s = p.read_number<decltype(s)>();
			if(p.read_char() != '%'){
				return style_value(style_value_special::none);
			}
			p.skip_whitespaces_and_comma();
			l = p.read_number<decltype(l)>();
			if(p.read_char() != '%'){
				return style_value(style_value_special::none);
			}
			p.skip_whitespaces();
			
			if(p.read_char() == ')'){
				auto color = hsl_to_rgb(real(h), real(s) / real(100), real(l) / real(100));
				return style_value(color);
			}
			return style_value(style_value_special::none);
		}
	}
	
	// check if color name
	{
		utki::string_parser p(str);
		auto name = p.read_word();
		
		auto i = color_name_to_color_map.find(name);
		if(i != color_name_to_color_map.end()){
			ASSERT(i->first == name)
			return style_value(i->second);
		}
	}
	
	return style_value(style_value_special::none);
}

std::string svgdom::paint_to_string(const style_value& v){
	if(std::holds_alternative<style_value_special>(v)){ // special value
		switch(*std::get_if<style_value_special>(&v)){
			case style_value_special::none:
				return none_word;
			case style_value_special::inherit: // TODO: isn't it already handled in styleable::style_value_to_string()?
				return inherit_word;
			case style_value_special::current_color:
				return current_color_word;
			default:
				return std::string();
		}
	}else if(std::holds_alternative<std::string>(v)){ // URL
		std::stringstream ss;
		ss << "url(" << *std::get_if<std::string>(&v) << ")";
		return ss.str();
	}else if(std::holds_alternative<uint32_t>(v)){
		auto i = color_to_color_name_map.find(*std::get_if<uint32_t>(&v));
		if(i != color_to_color_name_map.end()){
			// color name

			return std::string(i->second);
		}else{
			// #-notation

			std::stringstream s;
			s << std::hex;
			s << "#";
			s << ((*std::get_if<uint32_t>(&v) >> 4) & 0xf);
			s << ((*std::get_if<uint32_t>(&v)) & 0xf);
			s << ((*std::get_if<uint32_t>(&v) >> 12) & 0xf);
			s << ((*std::get_if<uint32_t>(&v) >> 8) & 0xf);
			s << ((*std::get_if<uint32_t>(&v) >> 20) & 0xf);
			s << ((*std::get_if<uint32_t>(&v) >> 16) & 0xf);
			return s.str();
		}
	}
	return std::string();
}

std::string svgdom::get_local_id_from_iri(const style_value& v){
	if(!std::holds_alternative<std::string>(v)){
		return std::string();
	}
	return iri_to_local_id(*std::get_if<std::string>(&v));
}
