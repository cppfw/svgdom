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

// TODO: remove when deprecated stuff is removed.
// some header defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

using namespace svgdom;

std::string styleable::styles_to_string() const {
	std::stringstream s;
	
	bool isFirst = true;
	
	for(auto& st : this->styles){
		if(isFirst){
			isFirst = false;
		}else{
			s << "; ";
		}
		
		ASSERT(st.first != style_property::unknown)
		
		s << propertyToString(st.first) << ":";
		
		if(st.second.type_ == style_value::type::inherit){
			s << "inherit";
			continue;
		}
		
		switch(st.first){
			default:
				TRACE(<< "Unimplemented style property: " << styleable::propertyToString(st.first) << ", writing empty value." << std::endl)
				break;
			case style_property::COLOR_INTERPOLATION_FILTERS:
				s << st.second.colorInterpolationFiltersToString();
				break;
			case style_property::STROKE_MITERLIMIT:
				s << st.second.stroke_miterlimit;
				break;
			case style_property::STOP_OPACITY:
			case style_property::OPACITY:
			case style_property::STROKE_OPACITY:
			case style_property::FILL_OPACITY:
				s << st.second.opacity;
				break;
			case style_property::STOP_COLOR:
			case style_property::FILL:
			case style_property::STROKE:
				s << st.second.paintToString();
				break;
			case style_property::STROKE_WIDTH:
				s << st.second.stroke_width;
				break;
			case style_property::STROKE_LINECAP:
				switch(st.second.stroke_line_cap){
					default:
						ASSERT(false)
						break;
					case stroke_line_cap::BUTT:
						s << "butt";
						break;
					case stroke_line_cap::ROUND:
						s << "round";
						break;
					case stroke_line_cap::SQUARE:
						s << "square";
						break;
				}
				break;
			case style_property::STROKE_LINEJOIN:
				switch(st.second.stroke_line_join){
					default:
						ASSERT(false)
						break;
					case stroke_line_join::MITER:
						s << "miter";
						break;
					case stroke_line_join::ROUND:
						s << "round";
						break;
					case stroke_line_join::BEVEL:
						s << "bevel";
						break;
				}
				break;
			case style_property::FILL_RULE:
				switch(st.second.fill_rule){
					default:
						ASSERT(false)
						break;
					case fill_rule::EVENODD:
						s << "evenodd";
						break;
					case fill_rule::NONZERO:
						s << "nonzero";
						break;
				}
				break;
			case style_property::MASK:
			case style_property::FILTER:
				s << "url(" << st.second.str << ")";
				break;
			case style_property::DISPLAY:
				s << st.second.displayToString();
				break;
			case style_property::ENABLE_BACKGROUND:
				s << st.second.enableBackgroundToString();
				break;
			case style_property::VISIBILITY:
				s << st.second.visibilityToString();
				break;
		}
	}
	return s.str();
}



//input parameter 'str' should have no leading or trailing white spaces
style_value styleable::parse_style_property_value(style_property type, const std::string& str){
	style_value v;

	if (str == "inherit") {
		v.type_ = style_value::type::inherit;
		return v;
	}

	switch(type){
		default:
			TRACE(<< "unimplemented style property encountered: " << styleable::propertyToString(type) << std::endl)
			break;
		case style_property::COLOR_INTERPOLATION_FILTERS:
			v = style_value::parseColorInterpolation(str);
			break;
		case style_property::STROKE_MITERLIMIT:
			{
				std::stringstream iss(str);
				v.stroke_miterlimit = readInReal(iss);
				v.type_ = style_value::type::normal;
				v.stroke_miterlimit = std::max(v.stroke_miterlimit, real(1)); // minimal value is 1
			}
			break;
		case style_property::STOP_OPACITY:
		case style_property::OPACITY:
		case style_property::STROKE_OPACITY:
		case style_property::FILL_OPACITY:
			{
				std::istringstream iss(str);
				v.opacity = readInReal(iss);
				utki::clampRange(v.opacity, real(0), real(1));
				v.type_ = style_value::type::normal;
			}
			break;
		case style_property::STOP_COLOR:
		case style_property::FILL:
		case style_property::STROKE:
			v = style_value::parsePaint(str);
//				TRACE(<< "paint read = " << std::hex << v.integer << std::endl)
			break;
		case style_property::STROKE_WIDTH:
			v.stroke_width = length::parse(str);
			v.type_ = style_value::type::normal;
//				TRACE(<< "stroke-width read = " << v.length << std::endl)
			break;
		case style_property::STROKE_LINECAP:
			v.type_ = style_value::type::normal;
			if(str == "butt"){
				v.stroke_line_cap = stroke_line_cap::BUTT;
			}else if(str == "round"){
				v.stroke_line_cap = stroke_line_cap::ROUND;
			}else if(str == "square"){
				v.stroke_line_cap = stroke_line_cap::SQUARE;
			}else{
				v.type_ = style_value::type::unknown;
				TRACE(<< "unknown strokeLineCap value:" << str << std::endl)
			}
			break;
		case style_property::STROKE_LINEJOIN:
			v.type_ = style_value::type::normal;
			if(str == "miter"){
				v.stroke_line_join = stroke_line_join::MITER;
			}else if(str == "round"){
				v.stroke_line_join = stroke_line_join::ROUND;
			}else if(str == "bevel"){
				v.stroke_line_join = stroke_line_join::BEVEL;
			}else{
				v.type_ = style_value::type::unknown;
				TRACE(<< "unknown strokeLineJoin value:" << str << std::endl)
			}
			break;
		case style_property::FILL_RULE:
			v.type_ = style_value::type::normal;
			if(str == "nonzero"){
				v.fill_rule = fill_rule::NONZERO;
			}else if(str == "evenodd"){
				v.fill_rule = fill_rule::EVENODD;
			}else{
				v.type_ = style_value::type::UNKNOWN;
				TRACE(<< "unknown fill-rule value:" << str << std::endl)
			}
			break;
		case style_property::MASK:
		case style_property::FILTER:
			v = style_value::parseUrl(str);
			break;
		case style_property::DISPLAY:
			v = style_value::parseDisplay(str);
			break;
		case style_property::ENABLE_BACKGROUND:
			v = style_value::parseEnableBackground(str);
			break;
		case style_property::VISIBILITY:
			v = style_value::parseVisibility(str);
			break;
	}
	return v;
}

style_value style_value::parse_url(const std::string& str) {
	style_value ret;
	ret.type_ = style_value::type::UNKNOWN;
	
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
		ret.type_ = style_value::type::URL;
	}

	return ret;
}

namespace{
style_value parseStylePropertyValue(style_property type, std::istream& s){
	skipWhitespaces(s);
	std::string str = readTillChar(s, ';');
	str = trimTail(str);
	return styleable::parseStylePropertyValue(type, str);
}
}

decltype(styleable::styles) styleable::parse(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(styleable::styles) ret;
	
	while(!s.eof()){
		std::string property = readTillCharOrWhitespace(s, ':');
		
		style_property type = styleable::stringToProperty(property);
		
		if(type == style_property::UNKNOWN){
			//unknown style property, skip it
			TRACE(<< "Unknown style property: " << property << std::endl)
			TRACE(<< "str = " << str << std::endl)
			TRACE(<< "ret.size() = " << ret.size() << std::endl)
			skipTillCharInclusive(s, ';');
			continue;
		}
		
		skipWhitespaces(s);
		
		if(s.get() != ':'){
			return ret;//expected colon
		}
		
		style_value v = ::parseStylePropertyValue(type, s);
		
		skipWhitespaces(s);
		
		if(!s.eof() && s.get() != ';'){
			return ret;//expected semicolon
		}
		
		ret[type] = std::move(v);
		
		skipWhitespaces(s);
	}
	
	return ret;
}

namespace{
const std::set<style_property> nonInheritedStyleProperties = {
	style_property::ALIGNMENT_BASELINE,
	style_property::BASELINE_SHIFT,
	style_property::CLIP,
	style_property::CLIP_PATH,
	style_property::DISPLAY,
	style_property::DOMINANT_BASELINE,
	style_property::ENABLE_BACKGROUND,
	style_property::FILTER,
	style_property::FLOOD_COLOR,
	style_property::FLOOD_OPACITY,
	style_property::LIGHTING_COLOR,
	style_property::MASK,
	style_property::OPACITY,
	style_property::OVERFLOW,
	style_property::STOP_COLOR,
	style_property::STOP_OPACITY,
	style_property::TEXT_DECORATION,
	style_property::UNICODE_BIDI
};
}

bool styleable::is_inherited(style_property p) {
	return nonInheritedStyleProperties.find(p) == nonInheritedStyleProperties.end();
}

namespace{
std::map<std::string, style_property> stringToPropertyMap = {
	{"alignment-baseline", style_property::ALIGNMENT_BASELINE},
	{"baseline-shift", style_property::BASELINE_SHIFT},
	{"clip", style_property::CLIP},
	{"clip-path", style_property::CLIP_PATH},
	{"clip-rule", style_property::CLIP_RULE},
	{"color", style_property::COLOR},
	{"color-interpolation", style_property::COLOR_INTERPOLATION},
	{"color-interpolation-filters", style_property::COLOR_INTERPOLATION_FILTERS},
	{"color-profile", style_property::COLOR_PROFILE},
	{"color-rendering", style_property::COLOR_RENDERING},
	{"cursor", style_property::CURSOR},
	{"direction", style_property::DIRECTION},
	{"display", style_property::DISPLAY},
	{"dominant-baseline", style_property::DOMINANT_BASELINE},
	{"enable-background", style_property::ENABLE_BACKGROUND},
	{"fill", style_property::FILL},
	{"fill-opacity", style_property::FILL_OPACITY},
	{"fill-rule", style_property::FILL_RULE},
	{"filter", style_property::FILTER},
	{"flood-color", style_property::FLOOD_COLOR},
	{"flood-opacity", style_property::FLOOD_OPACITY},
	{"font", style_property::FONT},
	{"font-family", style_property::FONT_FAMILY},
	{"font-size", style_property::FONT_SIZE},
	{"font-size-adjust", style_property::FONT_SIZE_ADJUST},
	{"font-stretch", style_property::FONT_STRETCH},
	{"font-style", style_property::FONT_STYLE},
	{"font-variant", style_property::FONT_VARIANT},
	{"font-weight", style_property::FONT_WEIGHT},
	{"glyph-orientation-horizontal", style_property::GLYPH_ORIENTATION_HORIZONTAL},
	{"glyph-orientation-vertical", style_property::GLYPH_ORIENTATION_VERTICAL},
	{"image-rendering", style_property::IMAGE_RENDERING},
	{"kerning", style_property::KERNING},
	{"letter-spacing", style_property::LETTER_SPACING},
	{"lighting-color", style_property::LIGHTING_COLOR},
	{"marker", style_property::MARKER},
	{"marker-end", style_property::MARKER_END},
	{"marker-mid", style_property::MARKER_MID},
	{"marker-start", style_property::MARKER_START},
	{"mask", style_property::MASK},
	{"opacity", style_property::OPACITY},
	{"overflow", style_property::OVERFLOW},
	{"pointer-events", style_property::POINTER_EVENTS},
	{"shape-rendering", style_property::SHAPE_RENDERING},
	{"stop-color", style_property::STOP_COLOR},
	{"stop-opacity", style_property::STOP_OPACITY},
	{"stroke", style_property::STROKE},
	{"stroke-dasharray", style_property::STROKE_DASHARRAY},
	{"stroke-dashoffset", style_property::STROKE_DASHOFFSET},
	{"stroke-linecap", style_property::STROKE_LINECAP},
	{"stroke-linejoin", style_property::STROKE_LINEJOIN},
	{"stroke-miterlimit", style_property::STROKE_MITERLIMIT},
	{"stroke-opacity", style_property::STROKE_OPACITY},
	{"stroke-width", style_property::STROKE_WIDTH},
	{"text-anchor", style_property::TEXT_ANCHOR},
	{"text-decoration", style_property::TEXT_DECORATION},
	{"text-rendering", style_property::TEXT_RENDERING},
	{"unicode-bidi", style_property::UNICODE_BIDI},
	{"visibility", style_property::VISIBILITY},
	{"word-spacing", style_property::WORD_SPACING},
	{"writing-mode", style_property::WRITING_MODE}
};
}

namespace{
auto propertytoStringMap = utki::flipMap(stringToPropertyMap);
}

style_property styleable::string_to_property(std::string str){
	auto i = stringToPropertyMap.find(str);
	if(i != stringToPropertyMap.end()){
		return i->second;
	}
	
	return style_property::UNKNOWN;
}

std::string styleable::property_to_string(style_property p){
	auto i = propertytoStringMap.find(p);
	if(i != propertytoStringMap.end()){
		return i->second;
	}
	return std::string();
}

const style_value* styleable::find_style_property(style_property p)const{
	auto i = this->styles.find(p);
	if(i != this->styles.end()){
		return &i->second;
	}
	return nullptr;
}

rgb style_value::get_rgb() const{
	auto c = this->color;
	
	rgb ret;
	
	ret.r = real(c & 0xff) / real(0xff);
	ret.g = real((c >> 8) & 0xff) / real(0xff);
	ret.b = real((c >> 16) & 0xff) / real(0xff);
	
	return ret;
}

void style_value::set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	this->color =  std::uint32_t(r) | (std::uint32_t(g) << 8) | (std::uint32_t(b) << 16);
}

namespace{
const std::map<std::string, std::uint32_t> colorNames = {
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
auto displayToStringMap = utki::flipMap(stringToDisplayMap);
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
	{"visible", visibility::VISIBLE},
	{"hidden", visibility::HIDDEN},
	{"collapse", visibility::COLLAPSE}
};
}

namespace{
auto visibilityToStringMap = utki::flipMap(stringToVisibilityMap);
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
	
	ret.type_ = style_value::type::NORMAL;
	
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
		ret.color_interpolation_filters = color_interpolation::AUTO;
	}else if(str == "linearRGB"){
		ret.color_interpolation_filters = color_interpolation::LINEAR_RGB;
	}else if(str == "sRGB"){
		ret.color_interpolation_filters = color_interpolation::S_RGB;
	}else{
		return ret;
	}
	
	ret.type_ = style_value::type::NORMAL;
	
	return ret;
}

namespace{
std::string colorInterpolationToString(color_interpolation ci){
	switch(ci){
		case color_interpolation::AUTO:
			return "auto";
		case color_interpolation::LINEAR_RGB:
			return "linearRGB";
		case color_interpolation::S_RGB:
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

std::string style_value::enable_background_to_string() const {
	switch(this->enable_background.value){
		default:
		case svgdom::enable_background::accumulate:
			return "accumulate";
		case svgdom::enable_background::new_:
			{
				std::stringstream ss;
				
				ss << "new";
				
				if(this->enable_background.isRectSpecified()){
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
std::uint32_t hslToRgb(real h, real s, real l){
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
	
	std::uint32_t ret = 0;
	
	ret |= std::uint32_t(r);
	ret |= (std::uint32_t(g) << 8);
	ret |= (std::uint32_t(b) << 16);
	
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
		ret = style_value::parseUrl(str);
		if(ret.isValid()){
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
				ret.color = (std::uint32_t(d[0]) << 4) | (std::uint32_t(d[0]))
						| (std::uint32_t(d[1]) << 12) | (std::uint32_t(d[1]) << 8)
						| (std::uint32_t(d[2]) << 20) | (std::uint32_t(d[2]) << 16);
				ret.type_ = style_value::type::normal;
				break;
			case 6:
				ret.color = (std::uint32_t(d[0]) << 4) | (std::uint32_t(d[1]))
						| (std::uint32_t(d[2]) << 12) | (std::uint32_t(d[3]) << 8)
						| (std::uint32_t(d[4]) << 20) | (std::uint32_t(d[5]) << 16);
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
			
			std::uint32_t r, g, b;
			
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
			
			std::uint32_t h, s, l;
			
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
