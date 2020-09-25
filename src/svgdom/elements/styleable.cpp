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

std::string styleable::style_value_to_string(style_property p, const style_value& v){
	if(v.type_ == style_value::type::inherit){
		return "inherit";
	}

	std::stringstream s;
	switch(p){
		default:
			TRACE(<< "Unimplemented style property: " << styleable::property_to_string(p) << ", writing empty value." << std::endl)
			break;
		case style_property::color_interpolation_filters:
			s << v.color_interpolation_filters_to_string();
			break;
		case style_property::stroke_miterlimit:
			s << v.stroke_miterlimit;
			break;
		case style_property::stop_opacity:
		case style_property::opacity:
		case style_property::stroke_opacity:
		case style_property::fill_opacity:
			s << v.opacity;
			break;
		case style_property::stop_color:
		case style_property::fill:
		case style_property::stroke:
			s << v.paint_to_string();
			break;
		case style_property::stroke_width:
			s << v.stroke_width;
			break;
		case style_property::stroke_linecap:
			switch(v.stroke_line_cap){
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
			break;
		case style_property::stroke_linejoin:
			switch(v.stroke_line_join){
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
			break;
		case style_property::fill_rule:
			switch(v.fill_rule){
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
			break;
		case style_property::mask:
		case style_property::filter:
			s << "url(" << v.str << ")";
			break;
		case style_property::display:
			s << v.display_to_string();
			break;
		case style_property::enable_background:
			s << v.enable_background_to_string();
			break;
		case style_property::visibility:
			s << v.visibility_to_string();
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
style_value styleable::parse_style_property_value(style_property type, const std::string& str){
	style_value v;

	if (str == "inherit") {
		v.type_ = style_value::type::inherit;
		return v;
	}

	switch(type){
		default:
			TRACE(<< "unimplemented style property encountered: " << styleable::property_to_string(type) << std::endl)
			break;
		case style_property::color_interpolation_filters:
			v = style_value::parse_color_interpolation(str);
			break;
		case style_property::stroke_miterlimit:
			{
				std::stringstream iss(str);
				v.stroke_miterlimit = readInReal(iss);
				v.type_ = style_value::type::normal;
				v.stroke_miterlimit = std::max(v.stroke_miterlimit, real(1)); // minimal value is 1
			}
			break;
		case style_property::stop_opacity:
		case style_property::opacity:
		case style_property::stroke_opacity:
		case style_property::fill_opacity:
			{
				std::istringstream iss(str);
				v.opacity = readInReal(iss);
				v.opacity = std::min(v.opacity, real(1)); // clamp top
				v.opacity = std::max(v.opacity, real(0)); // clamp bottom
				v.type_ = style_value::type::normal;
			}
			break;
		case style_property::stop_color:
		case style_property::fill:
		case style_property::stroke:
			v = style_value::parse_paint(str);
//				TRACE(<< "paint read = " << std::hex << v.integer << std::endl)
			break;
		case style_property::stroke_width:
			v.stroke_width = length::parse(str);
			v.type_ = style_value::type::normal;
//				TRACE(<< "stroke-width read = " << v.length << std::endl)
			break;
		case style_property::stroke_linecap:
			v.type_ = style_value::type::normal;
			if(str == "butt"){
				v.stroke_line_cap = stroke_line_cap::butt;
			}else if(str == "round"){
				v.stroke_line_cap = stroke_line_cap::round;
			}else if(str == "square"){
				v.stroke_line_cap = stroke_line_cap::square;
			}else{
				v.type_ = style_value::type::unknown;
				TRACE(<< "unknown strokeLineCap value:" << str << std::endl)
			}
			break;
		case style_property::stroke_linejoin:
			v.type_ = style_value::type::normal;
			if(str == "miter"){
				v.stroke_line_join = stroke_line_join::miter;
			}else if(str == "round"){
				v.stroke_line_join = stroke_line_join::round;
			}else if(str == "bevel"){
				v.stroke_line_join = stroke_line_join::bevel;
			}else{
				v.type_ = style_value::type::unknown;
				TRACE(<< "unknown strokeLineJoin value:" << str << std::endl)
			}
			break;
		case style_property::fill_rule:
			v.type_ = style_value::type::normal;
			if(str == "nonzero"){
				v.fill_rule = fill_rule::nonzero;
			}else if(str == "evenodd"){
				v.fill_rule = fill_rule::evenodd;
			}else{
				v.type_ = style_value::type::unknown;
				TRACE(<< "unknown fill-rule value:" << str << std::endl)
			}
			break;
		case style_property::mask:
		case style_property::filter:
			v = style_value::parse_url(str);
			break;
		case style_property::display:
			v = style_value::parse_display(str);
			break;
		case style_property::enable_background:
			v = style_value::parse_enable_background(str);
			break;
		case style_property::visibility:
			v = style_value::parse_visibility(str);
			break;
	}
	return v;
}

style_value style_value::parse_url(const std::string& str) {
	style_value ret;
	ret.type_ = style_value::type::unknown;
	
	std::string url = "url(";
	
	if(url != str.substr(0, url.length())){
		return ret;
	}
	
	std::istringstream s(str);
	skipWhitespaces(s);
	
	std::string tmpStr;
	s >> std::setw(int(url.length())) >> tmpStr >> std::setw(0);
	ASSERT(tmpStr == url)

	skipWhitespaces(s);
	tmpStr = readTillCharOrWhitespace(s, ')');

	skipWhitespaces(s);
	if(s.get() == ')'){
		ret.str = tmpStr;
		ret.type_ = style_value::type::url;
	}

	return ret;
}

namespace{
style_value parseStylePropertyValue(style_property type, std::istream& s){
	skipWhitespaces(s);
	std::string str = readTillChar(s, ';');
	str = trimTail(str);
	return styleable::parse_style_property_value(type, str);
}
}

decltype(styleable::styles) styleable::parse(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(styleable::styles) ret;
	
	while(!s.eof()){
		std::string property = readTillCharOrWhitespace(s, ':');
		
		style_property type = styleable::string_to_property(property);
		
		if(type == style_property::unknown){
			// unknown style property, skip it
			TRACE(<< "Unknown style property: " << property << std::endl)
			TRACE(<< "str = " << str << std::endl)
			TRACE(<< "ret.size() = " << ret.size() << std::endl)
			skipTillCharInclusive(s, ';');
			continue;
		}
		
		skipWhitespaces(s);
		
		if(s.get() != ':'){
			return ret; // expected colon
		}
		
		style_value v = ::parseStylePropertyValue(type, s);
		
		skipWhitespaces(s);
		
		if(!s.eof() && s.get() != ';'){
			return ret; // expected semicolon
		}
		
		ret[type] = std::move(v);
		
		skipWhitespaces(s);
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
const std::map<std::string, style_property> stringToPropertyMap = {
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
auto propertytoStringMap = utki::flip_map(stringToPropertyMap);
}

style_property styleable::string_to_property(std::string str){
	auto i = stringToPropertyMap.find(str);
	if(i != stringToPropertyMap.end()){
		return i->second;
	}
	
	return style_property::unknown;
}

std::string styleable::property_to_string(style_property p){
	auto i = propertytoStringMap.find(p);
	if(i != propertytoStringMap.end()){
		return i->second;
	}
	return std::string();
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

r4::vector3<real> style_value::get_rgb()const{
	auto c = this->color;
	
	r4::vector3<real> ret;
	
	ret.r() = real(c & 0xff) / real(0xff);
	ret.g() = real((c >> 8) & 0xff) / real(0xff);
	ret.b() = real((c >> 16) & 0xff) / real(0xff);
	
	return ret;
}

void style_value::set_rgb(uint8_t r, uint8_t g, uint8_t b){
	this->color = uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16);
}

void style_value::set_rgb(const r4::vector3<real>& rgb){
	auto c = (rgb * 0xff).to<uint8_t>();
	this->set_rgb(c.r(), c.g(), c.b());
}

namespace{
const std::map<std::string, uint32_t> colorNames = {
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
std::map<std::string, display> stringToDisplayMap = {
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
	{"none", svgdom::display::none}
};
}

namespace{
auto displayToStringMap = utki::flip_map(stringToDisplayMap);
}

style_value style_value::parse_display(const std::string& str) {
	style_value ret;
	
	// NOTE: "inherit" is already checked on upper level.
	
	auto i = stringToDisplayMap.find(str);
	if(i == stringToDisplayMap.end()){
		ret.display = svgdom::display::inline_; // default value
	}else{
		ret.display = i->second;
	}
	
	ret.type_ = style_value::type::normal;
	
	return ret;
}

std::string style_value::display_to_string()const{
	auto i = displayToStringMap.find(this->display);
	if(i == displayToStringMap.end()){
		return displayToStringMap[svgdom::display::inline_]; // default value
	}
	return i->second;
}

namespace{
std::map<std::string, visibility> stringToVisibilityMap = {
	{"visible", visibility::visible},
	{"hidden", visibility::hidden},
	{"collapse", visibility::collapse}
};
}

namespace{
auto visibilityToStringMap = utki::flip_map(stringToVisibilityMap);
}

style_value style_value::parse_visibility(const std::string& str){
	style_value ret;
	
	// NOTE: "inherit" is already checked on upper level.
	
	auto i = stringToVisibilityMap.find(str);
	if(i == stringToVisibilityMap.end()){
		ret.visibility = svgdom::visibility::visible; // default value
	}else{
		ret.visibility = i->second;
	}
	
	ret.type_ = style_value::type::normal;
	
	return ret;
}

std::string style_value::visibility_to_string()const{
	auto i = visibilityToStringMap.find(this->visibility);
	if(i == visibilityToStringMap.end()){
		return visibilityToStringMap[svgdom::visibility::visible]; // default value
	}
	return i->second;
}

style_value style_value::parse_color_interpolation(const std::string& str) {
	style_value ret;
	
	if(str == "auto"){
		ret.color_interpolation_filters = color_interpolation::auto_;
	}else if(str == "linearRGB"){
		ret.color_interpolation_filters = color_interpolation::linear_rgb;
	}else if(str == "sRGB"){
		ret.color_interpolation_filters = color_interpolation::s_rgb;
	}else{
		return ret;
	}
	
	ret.type_ = style_value::type::normal;
	
	return ret;
}

namespace{
std::string colorInterpolationToString(color_interpolation ci){
	switch(ci){
		case color_interpolation::auto_:
			return "auto";
		case color_interpolation::linear_rgb:
			return "linearRGB";
		case color_interpolation::s_rgb:
			return "sRGB";
		default:
			ASSERT_INFO(false, "ci = " << unsigned(ci))
			return "";
	}
}
}

std::string style_value::color_interpolation_filters_to_string() const {
	return colorInterpolationToString(this->color_interpolation_filters);
}

namespace{
enable_background_property parseEnableBackgroundNewRect(const std::string& str){
	enable_background_property ret;
	
	std::istringstream s(str);
	skipTillCharInclusive(s, ' '); // skip 'new'

	skipWhitespaces(s);
	
	if(s.eof()){
		ret.width = -1; // indicate that rectangle is not specified
		return ret;
	}
	
	ret.x = readInReal(s);
	if(s.fail()){
		throw malformed_svg_error("malformed 'enable-background NEW' string");
	}
	
	ret.y = readInReal(s);
	if(s.fail()){
		throw malformed_svg_error("malformed 'enable-background NEW' string");
	}
	
	ret.width = readInReal(s);
	if(s.fail()){
		throw malformed_svg_error("malformed enable-background NEW string");
	}
	
	ret.height = readInReal(s);
	if(s.fail()){
		throw malformed_svg_error("malformed enable-background NEW string");
	}
	
	return ret;
}
}

style_value style_value::parse_enable_background(const std::string& str) {
	style_value ret;
	
	std::string newStr = "new";
	if(str.substr(0, newStr.length()) == "new"){
		try{
			ret.enable_background = parseEnableBackgroundNewRect(str);
			ret.enable_background.value = svgdom::enable_background::new_;
		}catch(malformed_svg_error&){
			ret.enable_background.value = svgdom::enable_background::accumulate; // default value
		}
	}else{
		ret.enable_background.value = svgdom::enable_background::accumulate; // default value
	}
	
	ret.type_ = style_value::type::normal;
	
	return ret;
}

std::string style_value::enable_background_to_string()const{
	switch(this->enable_background.value){
		default:
		case svgdom::enable_background::accumulate:
			return "accumulate";
		case svgdom::enable_background::new_:
			{
				std::stringstream ss;
				
				ss << "new";
				
				if(this->enable_background.is_rect_specified()){
					ss << " " << this->enable_background.x;
					ss << " " << this->enable_background.y;
					ss << " " << this->enable_background.width;
					ss << " " << this->enable_background.height;
				}
				
				return ss.str();
			}
	}
}

namespace{
uint32_t hslToRgb(real h, real s, real l){
	real c = (real(1) - (std::abs(real(2) * l - real(1))) ) * s;
	real x = c * (1 - (std::abs(std::fmod(h / real(60), real(2)) - 1)));
	
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
	r *= 255;
	g *= 255;
	b *= 255;
	
	uint32_t ret = 0;
	
	ret |= uint32_t(r);
	ret |= (uint32_t(g) << 8);
	ret |= (uint32_t(b) << 16);
	
	return ret;
}
}

// 'str' should have no leading and/or trailing white spaces.
style_value style_value::parse_paint(const std::string& str){
	style_value ret;
	
	if(str.size() == 0){
		ret.type_ = style_value::type::none;
		return ret;
	}
	
	ASSERT(!std::isspace(str[0])) // leading spaces should be skept already	
	
	{
		ret = style_value::parse_url(str);
		if(ret.is_valid()){
			return ret;
		}
	}
	
	if(str == "none"){
		ret.type_ = style_value::type::none;
		return ret;
	}
	
	if(str == "currentColor"){
		ret.type_ = style_value::type::current_color;
		return ret;
	}
	
	// check if # notation
	if(str[0] == '#'){
		std::istringstream s(str.substr(1, 6));
		
		std::array<uint8_t, 6> d;
		unsigned numDigits = 0;
		for(auto i = d.begin(); i != d.end(); ++i, ++numDigits){
			char c = s.get();
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
		switch(numDigits){
			case 3:
				ret.color = (uint32_t(d[0]) << 4) | (uint32_t(d[0]))
						| (uint32_t(d[1]) << 12) | (uint32_t(d[1]) << 8)
						| (uint32_t(d[2]) << 20) | (uint32_t(d[2]) << 16);
				ret.type_ = style_value::type::normal;
				break;
			case 6:
				ret.color = (uint32_t(d[0]) << 4) | (uint32_t(d[1]))
						| (uint32_t(d[2]) << 12) | (uint32_t(d[3]) << 8)
						| (uint32_t(d[4]) << 20) | (uint32_t(d[5]) << 16);
				ret.type_ = style_value::type::normal;
				break;
			default:
				ret.type_ = style_value::type::none;
				break;
		}
		
//		TRACE(<< "# color read = " << std::hex << ret.integer << std::endl)
		return ret;
	}
	
	// check if rgb() or RGB() notation
	{
		const std::string rgb = "rgb(";
		if(rgb == str.substr(0, rgb.length())){
			std::istringstream s(str);
			
			std::string tmpStr;
			
			s >> std::setw(int(rgb.length())) >> tmpStr >> std::setw(0);
			ASSERT(tmpStr == rgb)
			
			uint32_t r, g, b;
			
			skipWhitespaces(s);
			s >> r;
			skipWhitespacesAndOrComma(s);
			s >> g;
			skipWhitespacesAndOrComma(s);
			s >> b;
			skipWhitespaces(s);
			
			if(s.get() == ')'){
				ret.color = r | (g << 8) | (b << 16);
				ret.type_ = style_value::type::normal;
			}
			return ret;
		}
	}
	
	// check if hsl() notation
	{
		const std::string hsl = "hsl(";
		if(hsl == str.substr(0, hsl.length())){
			std::istringstream ss(str);
			
			std::string tmpStr;
			
			ss >> std::setw(int(hsl.length())) >> tmpStr >> std::setw(0);
			ASSERT(tmpStr == hsl)
			
			uint32_t h, s, l;
			
			skipWhitespaces(ss);
			ss >> h;
			skipWhitespacesAndOrComma(ss);
			ss >> s;
			if(ss.get() != '%'){
				return ret;
			}
			skipWhitespacesAndOrComma(ss);
			ss >> l;
			if(ss.get() != '%'){
				return ret;
			}
			skipWhitespaces(ss);
			
			if(ss.get() == ')'){
				ret.color = hslToRgb(real(h), real(s) / real(100), real(l) / real(100));
				ret.type_ = style_value::type::normal;
			}
			return ret;
		}
	}
	
	// check if color name
	{
		std::istringstream s(str);
		std::string name;
		s >> name;
		
		auto i = colorNames.find(name);
		if(i != colorNames.end()){
			ASSERT(i->first == name)
			ret.str = name;
			ret.color = i->second;
			ret.type_ = style_value::type::normal;
			return ret;
		}
	}
	
	return ret;
}

std::string style_value::paint_to_string()const{
	switch(this->type_){
		default:
		case style_value::type::none:
			return "none";
		case style_value::type::inherit:
			return "inherit";
		case style_value::type::current_color:
			return "currentColor";
		case style_value::type::normal:
			if(this->str.size() == 0){
				// it is a # notation

				std::stringstream s;
				s << std::hex;
				s << "#";
				s << ((this->color >> 4) & 0xf);
				s << ((this->color) & 0xf);
				s << ((this->color >> 12) & 0xf);
				s << ((this->color >> 8) & 0xf);
				s << ((this->color >> 20) & 0xf);
				s << ((this->color >> 16) & 0xf);
				return s.str();
			}else{
				return this->str;
			}
		case style_value::type::url:
			{
				std::stringstream ss;
				ss << "url(" << this->str << ")";
				return ss.str();
			}
	}
}

std::string style_value::get_local_id_from_iri()const{
	return iriToLocalId(this->str);
}
