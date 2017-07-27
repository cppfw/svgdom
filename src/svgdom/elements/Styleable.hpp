#pragma once

#include <map>

#include "../config.hpp"
#include "../Length.hpp"

//math.h defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

namespace svgdom{

enum class StyleProperty_e{
	UNKNOWN,
	FONT,
	FONT_FAMILY,
	FONT_SIZE,
	FONT_SIZE_ADJUST,
	FONT_STRETCH,
	FONT_STYLE,
	FONT_VARIANT,
	FONT_WEIGHT,
	DIRECTION,
	LETTER_SPACING,
	TEXT_DECORATION,
	UNICODE_BIDI,
	WORD_SPACING,
	CLIP,
	COLOR,
	CURSOR,
	DISPLAY,
	OVERFLOW,
	VISIBILITY,
	CLIP_PATH,
    CLIP_RULE,
    MASK,
    OPACITY,
    ENABLE_BACKGROUND,
    FILTER,
    FLOOD_COLOR,
    FLOOD_OPACITY,
    LIGHTING_COLOR,
    STOP_COLOR,
    STOP_OPACITY,
    POINTER_EVENTS,
    COLOR_INTERPOLATION,
    COLOR_INTERPOLATION_FILTERS,
    COLOR_PROFILE,
    COLOR_RENDERING,
    FILL,
    FILL_OPACITY,
    FILL_RULE,
    IMAGE_RENDERING,
    MARKER,
    MARKER_END,
    MARKER_MID,
    MARKER_START,
    SHAPE_RENDERING,
    STROKE,
    STROKE_DASHARRAY,
    STROKE_DASHOFFSET,
    STROKE_LINECAP,
    STROKE_LINEJOIN,
    STROKE_MITERLIMIT,
    STROKE_OPACITY,
    STROKE_WIDTH,
    TEXT_RENDERING,
    ALIGNMENT_BASELINE,
    BASELINE_SHIFT,
    DOMINANT_BASELINE,
    GLYPH_ORIENTATION_HORIZONTAL,
    GLYPH_ORIENTATION_VERTICAL,
    KERNING,
    TEXT_ANCHOR,
    WRITING_MODE
};

/**
 * @brief Red, green and blue values.
 * All values are in range [0:1].
 */
struct Rgb{
	real r, g, b;
};

enum class StrokeLineCap_e{
	BUTT,
	ROUND,
	SQUARE
};

enum class StrokeLineJoin_e{
	MITER,
	ROUND,
	BEVEL
};

enum class FillRule_e{
	NONZERO,
	EVENODD
};

//TODO: remove
struct Element;

//TODO: rename to StyleValue
struct StylePropertyValue{
	enum class Type_e{
		/**
		 * @brief No special value is used.
		 */
		NORMAL,
		
		/**
		 * @brief For paint 'none' value.
		 * For paint property (e.g. fill, stroke, etc.) means that color is 'none'
		 */
		NONE,
		
		/**
		 * @brief For paint 'currentColor' value.
		 * Means "use 'color' property value".
		 */
		CURRENT_COLOR,
		
		/**
		 * @brief Property 'inherit' value.
		 * Means that property inheritance was explicitly stated using the 'inherit' keyword.
		 */
		INHERIT,
		
		/**
		 * @brief For paint 'url' value.
		 * Means that "str" member holds URL.
		 */
		URL
	} type = Type_e::NORMAL;
	
	bool isNormal()const noexcept{
		return this->type == Type_e::NORMAL;
	}
	
	bool isNone()const noexcept{
		return this->type == Type_e::NONE;
	}
	
	bool isUrl()const noexcept{
		return this->type == Type_e::URL;
	}
	
	union{
		std::uint32_t color;
		real opacity;
		Length length;
		StrokeLineCap_e strokeLineCap;
		StrokeLineJoin_e strokeLineJoin;
		FillRule_e fillRule;
		
		/**
		 * @brief reference to another element.
		 * Can be nullptr. It is used only if 'rule' is Type_e::URL.
		 */
		//TODO: remove
		Element* url; //used if rule is URL
	};
	
	/**
	 * @brief String data.
	 * This variable holds any string data which can be associated with the
	 * property value.
	 * In case the Type is URL it holds the url string.
	 * In case the Type is NORMAL and property value is a color specified by color name
	 * then it holds the color name.
	 */
	std::string str;
	
	/**
	 * @brief Get ID of the locally referenced element.
	 * If this value represents an URL this method will return local ID of the referenced element.
	 * @return ID of the locally referenced element.
	 * @return Empty string if this value is not URL or the reference is not local IRI.
	 */
	std::string getLocalIdFromIri()const;
	
	//TODO: move to cpp
	static StylePropertyValue parsePaint(const std::string& str);
	
	//TODO: move to cpp
	std::string paintToString()const;
	
	/**
	 * @brief get color as RGB.
	 * If this style property represents a color then this method returns the
	 * color as red, green and blue values.
	 * @return RGB structure holding red, green and blue.
	 */
	Rgb getRgb()const;
};

/**
 * @brief An element which has 'style' attribute or can be styled.
 */
struct Styleable{
	std::map<StyleProperty_e, StylePropertyValue> styles;
	
	const StylePropertyValue* findStyleProperty(StyleProperty_e p)const;
	
	void attribsToStream(std::ostream& s)const;
	
	//TODO: move to cpp
	static decltype(styles) parse(const std::string& str);
	
	//TODO: move to cpp
	static StylePropertyValue parseStylePropertyValue(StyleProperty_e type, const std::string& str);
	
	static bool isStylePropertyInherited(StyleProperty_e p);
	
	//TODO: move to cpp
	static std::string propertyToString(StyleProperty_e p);
	static StyleProperty_e stringToProperty(std::string str);
};

}
