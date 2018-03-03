#pragma once

#include <map>

#include "../config.hpp"
#include "../Length.hpp"

//math.h defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

namespace svgdom{

/**
 * @brief Style property names enumeration.
 */
enum class StyleProperty_e{
	ENUM_FIRST,
	UNKNOWN = ENUM_FIRST,		//0
	FONT,
	FONT_FAMILY,
	FONT_SIZE,
	FONT_SIZE_ADJUST,
	FONT_STRETCH,				//5
	FONT_STYLE,
	FONT_VARIANT,
	FONT_WEIGHT,
	DIRECTION,
	LETTER_SPACING,				//10
	TEXT_DECORATION,
	UNICODE_BIDI,
	WORD_SPACING,
	CLIP,
	COLOR,						//15
	CURSOR,
	DISPLAY,
	OVERFLOW,
	VISIBILITY,
	CLIP_PATH,					//20
	CLIP_RULE,
	MASK,
	OPACITY,
	ENABLE_BACKGROUND,
	FILTER,						//25
	FLOOD_COLOR,
	FLOOD_OPACITY,
	LIGHTING_COLOR,
	STOP_COLOR,
	STOP_OPACITY,				//30
	POINTER_EVENTS,
	COLOR_INTERPOLATION,
	COLOR_INTERPOLATION_FILTERS,
	COLOR_PROFILE,
	COLOR_RENDERING,			//35
	FILL,
	FILL_OPACITY,
	FILL_RULE,
	IMAGE_RENDERING,
	MARKER,						//40
	MARKER_END,
	MARKER_MID,
	MARKER_START,
	SHAPE_RENDERING,
	STROKE,						//45
	STROKE_DASHARRAY,
	STROKE_DASHOFFSET,
	STROKE_LINECAP,
	STROKE_LINEJOIN,
	STROKE_MITERLIMIT,			//50
	STROKE_OPACITY,
	STROKE_WIDTH,
	TEXT_RENDERING,
	ALIGNMENT_BASELINE,
	BASELINE_SHIFT,				//55
	DOMINANT_BASELINE,
	GLYPH_ORIENTATION_HORIZONTAL,
	GLYPH_ORIENTATION_VERTICAL,
	KERNING,
	TEXT_ANCHOR,				//60
	WRITING_MODE,
	
	ENUM_SIZE
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

/**
 * @brief Enumeration of color-interpolation-filters style property values.
 */
enum class ColorInterpolation_e{
	AUTO,
	S_RGB,
	LINEAR_RGB
};

enum class Display_e{
	INLINE,
	BLOCK,
	LIST_ITEM,
	RUN_IN,
	COMPACT,
	MARKER,
	TABLE,
	INLINE_TABLE,
	TABLE_ROW_GROUP,
	TABLE_HEADER_GROUP,
	TABLE_FOOTER_GROUP,
	TABLE_ROW,
	TABLE_COLUMN_GROUP,
	TABLE_COLUMN,
	TABLE_CELL,
	TABLE_CAPTION,
	NONE
};

enum class Visibility_e{
	VISIBLE,
	HIDDEN,
	COLLAPSE
};

enum class EnableBackground_e{
	ACCUMULATE,
	NEW
};

struct EnableBackground{
	EnableBackground_e value;
	real x, y, width, height; //these only make sense when 'value' is NEW.
	
	bool isRectSpecified()const noexcept{
		return this->width >= 0 && this->height >= 0;
	}
};

struct StyleValue{
	enum class Type_e{
		/**
		 * @brief Invalid style value.
		 */
		UNKNOWN,

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
	} type = Type_e::UNKNOWN;

	bool isValid()const noexcept{
		return this->type != Type_e::UNKNOWN;
	}

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
		real strokeMiterlimit;
		Length strokeWidth;
		StrokeLineCap_e strokeLineCap;
		StrokeLineJoin_e strokeLineJoin;
		FillRule_e fillRule;
		ColorInterpolation_e colorInterpolationFilters;
		Display_e display;
		EnableBackground enableBackground;
		Visibility_e visibility;
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

	static StyleValue parsePaint(const std::string& str);
	std::string paintToString()const;

	static StyleValue parseColorInterpolation(const std::string& str);
	
	static StyleValue parseDisplay(const std::string& str);
	std::string displayToString()const;

	static StyleValue parseVisibility(const std::string& str);
	std::string visibilityToString()const;
	
	static StyleValue parseEnableBackground(const std::string& str);
	std::string enableBackgroundToString()const;
	
	std::string colorInterpolationFiltersToString()const;

	static StyleValue parseUrl(const std::string& str);

	/**
	 * @brief get color as RGB.
	 * If this style property represents a color then this method returns the
	 * color as red, green and blue values.
	 * @return RGB structure holding red, green and blue.
	 */
	Rgb getRgb()const;
	
	/**
	 * @brief set color from RGB.
	 * If this style property represents a color then this method sets the
	 * color from red, green and blue values.
	 */
	void setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
};

/**
 * @brief An element which has 'style' attribute or can be styled.
 */
struct Styleable{
	std::map<StyleProperty_e, StyleValue> styles;

	const StyleValue* findStyleProperty(StyleProperty_e p)const;

	std::string stylesToString()const;

	static decltype(styles) parse(const std::string& str);

	static StyleValue parseStylePropertyValue(StyleProperty_e type, const std::string& str);

	static bool isStylePropertyInherited(StyleProperty_e p);

	static std::string propertyToString(StyleProperty_e p);
	static StyleProperty_e stringToProperty(std::string str);
};

}
