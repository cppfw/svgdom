#include "Styleable.hpp"

#include <set>
#include <iomanip>
#include <cctype>
#include <array>
#include <cmath>

#include <utki/debug.hpp>
#include <utki/util.hpp>

#include "../util.hxx"
#include "Element.hpp"
#include "../Exc.hpp"

//some header defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

using namespace svgdom;

std::string Styleable::stylesToString() const {
	std::stringstream s;
	
	bool isFirst = true;
	
	for(auto& st : this->styles){
		if(isFirst){
			isFirst = false;
		}else{
			s << "; ";
		}
		
		ASSERT(st.first != StyleProperty_e::UNKNOWN)
		
		s << propertyToString(st.first) << ":";
		
		if(st.second.type == StyleValue::Type_e::INHERIT){
			s << "inherit";
			continue;
		}
		
		switch(st.first){
			default:
				TRACE(<< "Unimplemented style property: " << Styleable::propertyToString(st.first) << ", writing empty value." << std::endl)
				break;
			case StyleProperty_e::COLOR_INTERPOLATION_FILTERS:
				s << st.second.colorInterpolationFiltersToString();
				break;
			case StyleProperty_e::STROKE_MITERLIMIT:
				s << st.second.strokeMiterlimit;
				break;
			case StyleProperty_e::STOP_OPACITY:
			case StyleProperty_e::OPACITY:
			case StyleProperty_e::STROKE_OPACITY:
			case StyleProperty_e::FILL_OPACITY:
				s << st.second.opacity;
				break;
			case StyleProperty_e::STOP_COLOR:
			case StyleProperty_e::FILL:
			case StyleProperty_e::STROKE:
				s << st.second.paintToString();
				break;
			case StyleProperty_e::STROKE_WIDTH:
				s << st.second.strokeWidth;
				break;
			case StyleProperty_e::STROKE_LINECAP:
				switch(st.second.strokeLineCap){
					default:
						ASSERT(false)
						break;
					case StrokeLineCap_e::BUTT:
						s << "butt";
						break;
					case StrokeLineCap_e::ROUND:
						s << "round";
						break;
					case StrokeLineCap_e::SQUARE:
						s << "square";
						break;
				}
				break;
			case StyleProperty_e::STROKE_LINEJOIN:
				switch(st.second.strokeLineJoin){
					default:
						ASSERT(false)
						break;
					case StrokeLineJoin_e::MITER:
						s << "miter";
						break;
					case StrokeLineJoin_e::ROUND:
						s << "round";
						break;
					case StrokeLineJoin_e::BEVEL:
						s << "bevel";
						break;
				}
				break;
			case StyleProperty_e::FILL_RULE:
				switch(st.second.fillRule){
					default:
						ASSERT(false)
						break;
					case FillRule_e::EVENODD:
						s << "evenodd";
						break;
					case FillRule_e::NONZERO:
						s << "nonzero";
						break;
				}
				break;
			case StyleProperty_e::MASK:
			case StyleProperty_e::FILTER:
				s << "url(" << st.second.str << ")";
				break;
			case StyleProperty_e::DISPLAY:
				s << st.second.displayToString();
				break;
			case StyleProperty_e::ENABLE_BACKGROUND:
				s << st.second.enableBackgroundToString();
				break;
			case StyleProperty_e::VISIBILITY:
				s << st.second.visibilityToString();
				break;
		}
	}
	return s.str();
}



//input parameter 'str' should have no leading or trailing white spaces
StyleValue Styleable::parseStylePropertyValue(StyleProperty_e type, const std::string& str){
	StyleValue v;

	if (str == "inherit") {
		v.type = StyleValue::Type_e::INHERIT;
		return v;
	}

	switch(type){
		default:
			TRACE(<< "unimplemented style property encountered: " << Styleable::propertyToString(type) << std::endl)
			break;
		case StyleProperty_e::COLOR_INTERPOLATION_FILTERS:
			v = StyleValue::parseColorInterpolation(str);
			break;
		case StyleProperty_e::STROKE_MITERLIMIT:
			{
				std::stringstream iss(str);
				v.strokeMiterlimit = readInReal(iss);
				v.type = StyleValue::Type_e::NORMAL;
				v.strokeMiterlimit = std::max(v.strokeMiterlimit, real(1));//minimal value is 1
			}
			break;
		case StyleProperty_e::STOP_OPACITY:
		case StyleProperty_e::OPACITY:
		case StyleProperty_e::STROKE_OPACITY:
		case StyleProperty_e::FILL_OPACITY:
			{
				std::istringstream iss(str);
				v.opacity = readInReal(iss);
				utki::clampRange(v.opacity, real(0), real(1));
				v.type = StyleValue::Type_e::NORMAL;
			}
			break;
		case StyleProperty_e::STOP_COLOR:
		case StyleProperty_e::FILL:
		case StyleProperty_e::STROKE:
			v = StyleValue::parsePaint(str);
//				TRACE(<< "paint read = " << std::hex << v.integer << std::endl)
			break;
		case StyleProperty_e::STROKE_WIDTH:
			v.strokeWidth = Length::parse(str);
			v.type = StyleValue::Type_e::NORMAL;
//				TRACE(<< "stroke-width read = " << v.length << std::endl)
			break;
		case StyleProperty_e::STROKE_LINECAP:
			v.type = StyleValue::Type_e::NORMAL;
			if(str == "butt"){
				v.strokeLineCap = StrokeLineCap_e::BUTT;
			}else if(str == "round"){
				v.strokeLineCap = StrokeLineCap_e::ROUND;
			}else if(str == "square"){
				v.strokeLineCap = StrokeLineCap_e::SQUARE;
			}else{
				v.type = StyleValue::Type_e::UNKNOWN;
				TRACE(<< "unknown strokeLineCap value:" << str << std::endl)
			}
			break;
		case StyleProperty_e::STROKE_LINEJOIN:
			v.type = StyleValue::Type_e::NORMAL;
			if(str == "miter"){
				v.strokeLineJoin = StrokeLineJoin_e::MITER;
			}else if(str == "round"){
				v.strokeLineJoin = StrokeLineJoin_e::ROUND;
			}else if(str == "bevel"){
				v.strokeLineJoin = StrokeLineJoin_e::BEVEL;
			}else{
				v.type = StyleValue::Type_e::UNKNOWN;
				TRACE(<< "unknown strokeLineJoin value:" << str << std::endl)
			}
			break;
		case StyleProperty_e::FILL_RULE:
			v.type = StyleValue::Type_e::NORMAL;
			if(str == "nonzero"){
				v.fillRule = FillRule_e::NONZERO;
			}else if(str == "evenodd"){
				v.fillRule = FillRule_e::EVENODD;
			}else{
				v.type = StyleValue::Type_e::UNKNOWN;
				TRACE(<< "unknown fill-rule value:" << str << std::endl)
			}
			break;
		case StyleProperty_e::MASK:
		case StyleProperty_e::FILTER:
			v = StyleValue::parseUrl(str);
			break;
		case StyleProperty_e::DISPLAY:
			v = StyleValue::parseDisplay(str);
			break;
		case StyleProperty_e::ENABLE_BACKGROUND:
			v = StyleValue::parseEnableBackground(str);
			break;
		case StyleProperty_e::VISIBILITY:
			v = StyleValue::parseVisibility(str);
			break;
	}
	return v;
}

StyleValue StyleValue::parseUrl(const std::string& str) {
	StyleValue ret;
	ret.type = StyleValue::Type_e::UNKNOWN;
	
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
		ret.type = StyleValue::Type_e::URL;
	}

	return ret;
}



namespace{
StyleValue parseStylePropertyValue(StyleProperty_e type, std::istream& s){
	skipWhitespaces(s);
	std::string str = readTillChar(s, ';');
	str = trimTail(str);
	return Styleable::parseStylePropertyValue(type, str);
}
}

decltype(Styleable::styles) Styleable::parse(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(Styleable::styles) ret;
	
	while(!s.eof()){
		std::string property = readTillCharOrWhitespace(s, ':');
		
		StyleProperty_e type = Styleable::stringToProperty(property);
		
		if(type == StyleProperty_e::UNKNOWN){
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
		
		StyleValue v = ::parseStylePropertyValue(type, s);
		
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
const std::set<StyleProperty_e> nonInheritedStyleProperties = {
	StyleProperty_e::ALIGNMENT_BASELINE,
	StyleProperty_e::BASELINE_SHIFT,
	StyleProperty_e::CLIP,
	StyleProperty_e::CLIP_PATH,
	StyleProperty_e::DISPLAY,
	StyleProperty_e::DOMINANT_BASELINE,
	StyleProperty_e::ENABLE_BACKGROUND,
	StyleProperty_e::FILTER,
	StyleProperty_e::FLOOD_COLOR,
	StyleProperty_e::FLOOD_OPACITY,
	StyleProperty_e::LIGHTING_COLOR,
	StyleProperty_e::MASK,
	StyleProperty_e::OPACITY,
	StyleProperty_e::OVERFLOW,
	StyleProperty_e::STOP_COLOR,
	StyleProperty_e::STOP_OPACITY,
	StyleProperty_e::TEXT_DECORATION,
	StyleProperty_e::UNICODE_BIDI
};
}

bool Styleable::isStylePropertyInherited(StyleProperty_e p) {
	return nonInheritedStyleProperties.find(p) == nonInheritedStyleProperties.end();
}

namespace{
std::map<std::string, StyleProperty_e> stringToPropertyMap = {
	{"alignment-baseline", StyleProperty_e::ALIGNMENT_BASELINE},
	{"baseline-shift", StyleProperty_e::BASELINE_SHIFT},
	{"clip", StyleProperty_e::CLIP},
	{"clip-path", StyleProperty_e::CLIP_PATH},
	{"clip-rule", StyleProperty_e::CLIP_RULE},
	{"color", StyleProperty_e::COLOR},
	{"color-interpolation", StyleProperty_e::COLOR_INTERPOLATION},
	{"color-interpolation-filters", StyleProperty_e::COLOR_INTERPOLATION_FILTERS},
	{"color-profile", StyleProperty_e::COLOR_PROFILE},
	{"color-rendering", StyleProperty_e::COLOR_RENDERING},
	{"cursor", StyleProperty_e::CURSOR},
	{"direction", StyleProperty_e::DIRECTION},
	{"display", StyleProperty_e::DISPLAY},
	{"dominant-baseline", StyleProperty_e::DOMINANT_BASELINE},
	{"enable-background", StyleProperty_e::ENABLE_BACKGROUND},
	{"fill", StyleProperty_e::FILL},
	{"fill-opacity", StyleProperty_e::FILL_OPACITY},
	{"fill-rule", StyleProperty_e::FILL_RULE},
	{"filter", StyleProperty_e::FILTER},
	{"flood-color", StyleProperty_e::FLOOD_COLOR},
	{"flood-opacity", StyleProperty_e::FLOOD_OPACITY},
	{"font", StyleProperty_e::FONT},
	{"font-family", StyleProperty_e::FONT_FAMILY},
	{"font-size", StyleProperty_e::FONT_SIZE},
	{"font-size-adjust", StyleProperty_e::FONT_SIZE_ADJUST},
	{"font-stretch", StyleProperty_e::FONT_STRETCH},
	{"font-style", StyleProperty_e::FONT_STYLE},
	{"font-variant", StyleProperty_e::FONT_VARIANT},
	{"font-weight", StyleProperty_e::FONT_WEIGHT},
	{"glyph-orientation-horizontal", StyleProperty_e::GLYPH_ORIENTATION_HORIZONTAL},
	{"glyph-orientation-vertical", StyleProperty_e::GLYPH_ORIENTATION_VERTICAL},
	{"image-rendering", StyleProperty_e::IMAGE_RENDERING},
	{"kerning", StyleProperty_e::KERNING},
	{"letter-spacing", StyleProperty_e::LETTER_SPACING},
	{"lighting-color", StyleProperty_e::LIGHTING_COLOR},
	{"marker", StyleProperty_e::MARKER},
	{"marker-end", StyleProperty_e::MARKER_END},
	{"marker-mid", StyleProperty_e::MARKER_MID},
	{"marker-start", StyleProperty_e::MARKER_START},
	{"mask", StyleProperty_e::MASK},
	{"opacity", StyleProperty_e::OPACITY},
	{"overflow", StyleProperty_e::OVERFLOW},
	{"pointer-events", StyleProperty_e::POINTER_EVENTS},
	{"shape-rendering", StyleProperty_e::SHAPE_RENDERING},
	{"stop-color", StyleProperty_e::STOP_COLOR},
	{"stop-opacity", StyleProperty_e::STOP_OPACITY},
	{"stroke", StyleProperty_e::STROKE},
	{"stroke-dasharray", StyleProperty_e::STROKE_DASHARRAY},
	{"stroke-dashoffset", StyleProperty_e::STROKE_DASHOFFSET},
	{"stroke-linecap", StyleProperty_e::STROKE_LINECAP},
	{"stroke-linejoin", StyleProperty_e::STROKE_LINEJOIN},
	{"stroke-miterlimit", StyleProperty_e::STROKE_MITERLIMIT},
	{"stroke-opacity", StyleProperty_e::STROKE_OPACITY},
	{"stroke-width", StyleProperty_e::STROKE_WIDTH},
	{"text-anchor", StyleProperty_e::TEXT_ANCHOR},
	{"text-decoration", StyleProperty_e::TEXT_DECORATION},
	{"text-rendering", StyleProperty_e::TEXT_RENDERING},
	{"unicode-bidi", StyleProperty_e::UNICODE_BIDI},
	{"visibility", StyleProperty_e::VISIBILITY},
	{"word-spacing", StyleProperty_e::WORD_SPACING},
	{"writing-mode", StyleProperty_e::WRITING_MODE}
};
}

namespace{
auto propertytoStringMap = utki::flipMap(stringToPropertyMap);
}

StyleProperty_e Styleable::stringToProperty(std::string str){
	auto i = stringToPropertyMap.find(str);
	if(i != stringToPropertyMap.end()){
		return i->second;
	}
	
	return StyleProperty_e::UNKNOWN;
}

std::string Styleable::propertyToString(StyleProperty_e p){
	auto i = propertytoStringMap.find(p);
	if(i != propertytoStringMap.end()){
		return i->second;
	}
	return std::string();
}

const StyleValue* Styleable::findStyleProperty(StyleProperty_e p)const{
	auto i = this->styles.find(p);
	if(i != this->styles.end()){
		return &i->second;
	}
	return nullptr;
}

Rgb StyleValue::getRgb() const{
	auto c = this->color;
	
	Rgb ret;
	
	ret.r = real(c & 0xff) / real(0xff);
	ret.g = real((c >> 8) & 0xff) / real(0xff);
	ret.b = real((c >> 16) & 0xff) / real(0xff);
	
	return ret;
}

void StyleValue::setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
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
std::map<std::string, Display_e> stringToDisplayMap = {
	{"inline", Display_e::INLINE},
	{"block", Display_e::BLOCK},
	{"list-item", Display_e::LIST_ITEM},
	{"run-in", Display_e::RUN_IN},
	{"compact", Display_e::COMPACT},
	{"marker", Display_e::MARKER},
	{"table", Display_e::TABLE},
	{"inline-table", Display_e::INLINE_TABLE},
	{"table-row-group", Display_e::TABLE_ROW_GROUP},
	{"table-header-group", Display_e::TABLE_HEADER_GROUP},
	{"table-footer-group", Display_e::TABLE_FOOTER_GROUP},
	{"table-row", Display_e::TABLE_ROW},
	{"table-column-group", Display_e::TABLE_COLUMN_GROUP},
	{"table-column", Display_e::TABLE_COLUMN},
	{"table-cell", Display_e::TABLE_CELL},
	{"table-caption", Display_e::TABLE_CAPTION},
	{"none", Display_e::NONE}
};
}

namespace{
auto displayToStringMap = utki::flipMap(stringToDisplayMap);
}

StyleValue StyleValue::parseDisplay(const std::string& str) {
	StyleValue ret;
	
	//NOTE: "inherit" is already checked on upper level.
	
	auto i = stringToDisplayMap.find(str);
	if(i == stringToDisplayMap.end()){
		ret.display = Display_e::INLINE; // default value
	}else{
		ret.display = i->second;
	}
	
	ret.type = StyleValue::Type_e::NORMAL;
	
	return ret;
}

std::string StyleValue::displayToString()const {
	auto i = displayToStringMap.find(this->display);
	if(i == displayToStringMap.end()){
		return displayToStringMap[Display_e::INLINE]; //default value
	}
	return i->second;
}

namespace{
std::map<std::string, Visibility_e> stringToVisibilityMap = {
	{"visible", Visibility_e::VISIBLE},
	{"hidden", Visibility_e::HIDDEN},
	{"collapse", Visibility_e::COLLAPSE}
};
}

namespace{
auto visibilityToStringMap = utki::flipMap(stringToVisibilityMap);
}

StyleValue StyleValue::parseVisibility(const std::string& str){
	StyleValue ret;
	
	//NOTE: "inherit" is already checked on upper level.
	
	auto i = stringToVisibilityMap.find(str);
	if(i == stringToVisibilityMap.end()){
		ret.visibility = Visibility_e::VISIBLE; // default value
	}else{
		ret.visibility = i->second;
	}
	
	ret.type = StyleValue::Type_e::NORMAL;
	
	return ret;
}

std::string StyleValue::visibilityToString() const {
	auto i = visibilityToStringMap.find(this->visibility);
	if(i == visibilityToStringMap.end()){
		return visibilityToStringMap[Visibility_e::VISIBLE]; //default value
	}
	return i->second;
}


StyleValue StyleValue::parseColorInterpolation(const std::string& str) {
	StyleValue ret;
	
	if(str == "auto"){
		ret.colorInterpolationFilters = ColorInterpolation_e::AUTO;
	}else if(str == "linearRGB"){
		ret.colorInterpolationFilters = ColorInterpolation_e::LINEAR_RGB;
	}else if(str == "sRGB"){
		ret.colorInterpolationFilters = ColorInterpolation_e::S_RGB;
	}else{
		return ret;
	}
	
	ret.type = StyleValue::Type_e::NORMAL;
	
	return ret;
}

namespace{
std::string colorInterpolationToString(ColorInterpolation_e ci){
	switch(ci){
		case ColorInterpolation_e::AUTO:
			return "auto";
		case ColorInterpolation_e::LINEAR_RGB:
			return "linearRGB";
		case ColorInterpolation_e::S_RGB:
			return "sRGB";
		default:
			ASSERT_INFO(false, "ci = " << unsigned(ci))
			return "";
	}
}
}

std::string StyleValue::colorInterpolationFiltersToString() const {
	return colorInterpolationToString(this->colorInterpolationFilters);
}

namespace{
EnableBackground parseEnableBackgroundNewRect(const std::string& str){
	EnableBackground ret;
	
	std::istringstream s(str);
	skipTillCharInclusive(s, ' '); //skip 'new'

	skipWhitespaces(s);
	
	if(s.eof()){
		ret.width = -1; //indicate that rectangle is not specified
		return ret;
	}
	
	ret.x = readInReal(s);
	if(s.fail()){
		throw svgdom::Exc("malformed enable-background NEW string");
	}
	
	ret.y = readInReal(s);
	if(s.fail()){
		throw svgdom::Exc("malformed enable-background NEW string");
	}
	
	ret.width = readInReal(s);
	if(s.fail()){
		throw svgdom::Exc("malformed enable-background NEW string");
	}
	
	ret.height = readInReal(s);
	if(s.fail()){
		throw svgdom::Exc("malformed enable-background NEW string");
	}
	
	return ret;
}
}

StyleValue StyleValue::parseEnableBackground(const std::string& str) {
	StyleValue ret;
	
	std::string newStr = "new";
	if(str.substr(0, newStr.length()) == "new"){
		try{
			ret.enableBackground = parseEnableBackgroundNewRect(str);
			ret.enableBackground.value = EnableBackground_e::NEW;
		}catch(svgdom::Exc& e){
			ret.enableBackground.value = EnableBackground_e::ACCUMULATE; //default value
		}
	}else{
		ret.enableBackground.value = EnableBackground_e::ACCUMULATE; //default value
	}
	
	ret.type = StyleValue::Type_e::NORMAL;
	
	return ret;
}


std::string StyleValue::enableBackgroundToString() const {
	switch(this->enableBackground.value){
		default:
		case EnableBackground_e::ACCUMULATE:
			return "accumulate";
		case EnableBackground_e::NEW:
			{
				std::stringstream ss;
				
				ss << "new";
				
				if(this->enableBackground.isRectSpecified()){
					ss << " " << this->enableBackground.x;
					ss << " " << this->enableBackground.y;
					ss << " " << this->enableBackground.width;
					ss << " " << this->enableBackground.height;
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

//'str' should have no leading and/or trailing white spaces.
StyleValue StyleValue::parsePaint(const std::string& str){
	StyleValue ret;
	
	if(str.size() == 0){
		ret.type = StyleValue::Type_e::NONE;
		return ret;
	}
	
	ASSERT(!std::isspace(str[0])) //leading spaces should be skept already	
	
	{
		ret = StyleValue::parseUrl(str);
		if(ret.isValid()){
			return ret;
		}
	}
	
	if(str == "none"){
		ret.type = StyleValue::Type_e::NONE;
		return ret;
	}
	
	if(str == "currentColor"){
		ret.type = StyleValue::Type_e::CURRENT_COLOR;
		return ret;
	}
	
	//check if # notation
	if(str[0] == '#'){
		std::istringstream s(str.substr(1, 6));
		
		std::array<std::uint8_t, 6> d;
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
			
			ASSERT(((*i) & 0xf0) == 0) //only one hex digit
		}
		switch(numDigits){
			case 3:
				ret.color = (std::uint32_t(d[0]) << 4) | (std::uint32_t(d[0]))
						| (std::uint32_t(d[1]) << 12) | (std::uint32_t(d[1]) << 8)
						| (std::uint32_t(d[2]) << 20) | (std::uint32_t(d[2]) << 16);
				ret.type = StyleValue::Type_e::NORMAL;
				break;
			case 6:
				ret.color = (std::uint32_t(d[0]) << 4) | (std::uint32_t(d[1]))
						| (std::uint32_t(d[2]) << 12) | (std::uint32_t(d[3]) << 8)
						| (std::uint32_t(d[4]) << 20) | (std::uint32_t(d[5]) << 16);
				ret.type = StyleValue::Type_e::NORMAL;
				break;
			default:
				ret.type = StyleValue::Type_e::NONE;
				break;
		}
		
//		TRACE(<< "# color read = " << std::hex << ret.integer << std::endl)
		return ret;
	}
	
	//check if rgb() or RGB() notation
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
				ret.type = StyleValue::Type_e::NORMAL;
			}
			return ret;
		}
	}
	
	//check if hsl() notation
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
				ret.type = StyleValue::Type_e::NORMAL;
			}
			return ret;
		}
	}
	
	//check if color name
	{
		std::istringstream s(str);
		std::string name;
		s >> name;
		
		auto i = colorNames.find(name);
		if(i != colorNames.end()){
			ASSERT(i->first == name)
			ret.str = name;
			ret.color = i->second;
			ret.type = StyleValue::Type_e::NORMAL;
			return ret;
		}
	}
	
	return ret;
}

std::string StyleValue::paintToString()const{
	switch(this->type){
		default:
		case Type_e::NONE:
			return "none";
		case Type_e::INHERIT:
			return "inherit";
		case Type_e::CURRENT_COLOR:
			return "currentColor";
		case Type_e::NORMAL:
			if(this->str.size() == 0){
				//it is a # notation

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
		case Type_e::URL:
			{
				std::stringstream ss;
				ss << "url(" << this->str << ")";
				return ss.str();
			}
	}
}

std::string StyleValue::getLocalIdFromIri() const {
	return iriToLocalId(this->str);
}
