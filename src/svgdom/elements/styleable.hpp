#pragma once

#include <map>
#include <vector>

#include <cssdom/dom.hpp>

#include "../config.hpp"
#include "../length.hpp"

// TODO: remove this when deprecated stuff is removed.
// math.h defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

namespace svgdom{

/**
 * @brief Style property names enumeration.
 */
enum class style_property : uint32_t{
	unknown,					// 0
	UNKNOWN = unknown, // TODO: deprecated, remove.
	font,
	FONT = font, // TODO: deprecated, remove.
	font_family,
	FONT_FAMILY = font_family, // TODO: deprecated, remove.
	font_size,
	FONT_SIZE = font_size, // TODO: deprecated, remove.
	font_size_adjust,
	FONT_SIZE_ADJUST = font_size_adjust, // TODO: deprecated, remove.
	font_stretch,				// 5
	FONT_STRETCH = font_stretch, // TODO: deprecated, remove.
	font_style,
	FONT_STYLE = font_style, // TODO: deprecated, remove.
	font_variant,
	FONT_VARIANT = font_variant, // TODO: deprecated, remove.
	font_weight,
	FONT_WEIGHT = font_weight, // TODO: deprecated, remove.
	direction,
	DIRECTION = direction, // TODO: deprecated, remove.
	letter_spacing,				// 10
	LETTER_SPACING = letter_spacing, // TODO: deprecated, remove.
	text_decoration,
	TEXT_DECORATION = text_decoration, // TODO: deprecated, remove.
	unicode_bidi,
	UNICODE_BIDI = unicode_bidi, // TODO: deprecated, remove.
	word_spacing,
	WORD_SPACING = word_spacing, // TODO: deprecated, remove.
	clip,
	CLIP = clip, // TODO: deprecated, remove.
	color,						// 15
	COLOR = color, // TODO: deprecated, remove.
	cursor,
	CURSOR = cursor, // TODO: deprecated, remove.
	display,
	DISPLAY = display, // TODO: deprecated, remove.
	overflow,
	OVERFLOW = overflow, // TODO: deprecated, remove.
	visibility,
	VISIBILITY = visibility, // TODO: deprecated, remove.
	clip_path,					// 20
	CLIP_PATH = clip_path, // TODO: deprecated, remove.
	clip_rule,
	CLIP_RULE = clip_rule, // TODO: deprecated, remove.
	mask,
	MASK = mask, // TODO: deprecated, remove.
	opacity,
	OPACITY = opacity, // TODO: deprecated, remove.
	enable_background,
	ENABLE_BACKGROUND = enable_background, // TODO: deprecated, remove.
	filter,						// 25
	FILTER = filter, // TODO: deprecated, remove.
	flood_color,
	FLOOD_COLOR = flood_color, // TODO: deprecated, remove.
	flood_opacity,
	FLOOD_OPACITY = flood_opacity, // TODO: deprecated, remove.
	lighting_color,
	LIGHTING_COLOR = lighting_color, // TODO: deprecated, remove.
	stop_color,
	STOP_COLOR = stop_color, // TODO: deprecated, remove.
	stop_opacity,				// 30
	STOP_OPACITY = stop_opacity, // TODO: deprecated, remove.
	pointer_events,
	POINTER_EVENTS = pointer_events, // TODO: deprecated, remove.
	color_interpolation,
	COLOR_INTERPOLATION = color_interpolation, // TODO: deprecated, remove.
	color_interpolation_filters,
	COLOR_INTERPOLATION_FILTERS = color_interpolation_filters, // TODO: deprecated, remove.
	color_profile,
	COLOR_PROFILE = color_profile, // TODO: deprecated, remove.
	color_rendering,			// 35
	COLOR_RENDERING = color_rendering, // TODO: deprecated, remove.
	fill,
	FILL = fill, // TODO: deprecated, remove.
	fill_opacity,
	FILL_OPACITY = fill_opacity, // TODO: deprecated, remove.
	fill_rule,
	FILL_RULE = fill_rule, // TODO: deprecated, remove.
	image_rendering,
	IMAGE_RENDERING = image_rendering, // TODO: deprecated, remove.
	marker,						// 40
	MARKER = marker, // TODO: deprecated, remove.
	marker_end,
	MARKER_END = marker_end, // TODO: deprecated, remove.
	marker_mid,
	MARKER_MID = marker_mid, // TODO: deprecated, remove.
	marker_start,
	MARKER_START = marker_start, // TODO: deprecated, remove.
	shape_rendering,
	SHAPE_RENDERING = shape_rendering, // TODO: deprecated, remove.
	stroke,						// 45
	STROKE = stroke, // TODO: deprecated, remove.
	stroke_dasharray,
	STROKE_DASHARRAY = stroke_dasharray, // TODO: deprecated, remove.
	stroke_dashoffset,
	STROKE_DASHOFFSET = stroke_dashoffset, // TODO: deprecated, remove.
	stroke_linecap,
	STROKE_LINECAP = stroke_linecap, // TODO: deprecated, remove.
	stroke_linejoin,
	STROKE_LINEJOIN = stroke_linejoin, // TODO: deprecated, remove.
	stroke_miterlimit,			// 50
	STROKE_MITERLIMIT = stroke_miterlimit, // TODO: deprecated, remove.
	stroke_opacity,
	STROKE_OPACITY = stroke_opacity, // TODO: deprecated, remove.
	stroke_width,
	STROKE_WIDTH = stroke_width, // TODO: deprecated, remove.
	text_rendering,
	TEXT_RENDERING = text_rendering, // TODO: deprecated, remove.
	alignment_baseline,
	ALIGNMENT_BASELINE = alignment_baseline, // TODO: deprecated, remove.
	baseline_shift,				// 55
	BASELINE_SHIFT = baseline_shift, // TODO: deprecated, remove.
	dominant_baseline,
	DOMINANT_BASELINE = dominant_baseline, // TODO: deprecated, remove.
	glyph_orientation_horizontal,
	GLYPH_ORIENTATION_HORIZONTAL = glyph_orientation_horizontal, // TODO: deprecated, remove.
	glyph_orientation_vertical,
	GLYPH_ORIENTATION_VERTICAL = glyph_orientation_vertical, // TODO: deprecated, remove.
	kerning,
	KERNING = kerning, // TODO: deprecated, remove.
	text_anchor,				// 60
	TEXT_ANCHOR = text_anchor, // TODO: deprecated, remove.
	writing_mode,
	WRITING_MODE = writing_mode, // TODO: deprecated, remove.
	
	ENUM_SIZE
};

// TODO: deprecated, remove.
typedef style_property StyleProperty_e;

/**
 * @brief Red, green and blue values.
 * All values are in range [0:1].
 */
struct rgb{
	real r, g, b;
};

// TODO: deprecated, remove.
typedef rgb Rgb;

enum class stroke_line_cap{
	butt,
	BUTT = butt, // TODO: deprecated, remove.
	round,
	ROUND = round, // TODO: deprecated, remove.
	square,
	SQUARE = square // TODO: deprecated, remove.
};

// TODO: deprecated, remove.
typedef stroke_line_cap StrokeLineCap_e;

enum class stroke_line_join{
	miter,
	MITER = miter, // TODO: deprecated, remove.
	round,
	ROUND = round, // TODO: deprecated, remove.
	bevel,
	BEVEL = bevel // TODO: deprecated, remove.
};

// TODO: deprecated, remove.
typedef stroke_line_join StrokeLineJoin_e;

enum class fill_rule{
	nonzero,
	NONZERO = nonzero, // TODO: deprecated, remove.
	evenodd,
	EVENODD = evenodd // TODO: deprecated, remove.
};

// TODO: deprecated, remove.
typedef fill_rule FillRule_e;

/**
 * @brief Enumeration of color-interpolation-filters style property values.
 */
enum class color_interpolation{
	auto_,
	AUTO = auto_, // TODO: deprecated, remove.
	s_rgb,
	S_RGB = s_rgb, // TODO: deprecated, remove.
	linear_rgb,
	LINEAR_RGB = linear_rgb // TODO: deprecated, remove.
};

// TODO: deprecated, remove.
typedef color_interpolation ColorInterpolation_e;

enum class display{
	inline_,
	INLINE = inline_, // TODO: derprecated, remove.
	block,
	BLOCK = block, // TODO: derprecated, remove.
	list_item,
	LIST_ITEM = list_item, // TODO: derprecated, remove.
	run_in,
	RUN_IN = run_in, // TODO: derprecated, remove.
	compact,
	COMPACT = compact, // TODO: derprecated, remove.
	marker,
	MARKER = marker, // TODO: derprecated, remove.
	table,
	TABLE = table, // TODO: derprecated, remove.
	inline_table,
	INLINE_TABLE = inline_table, // TODO: derprecated, remove.
	table_row_group,
	TABLE_ROW_GROUP = table_row_group, // TODO: derprecated, remove.
	table_header_group,
	TABLE_HEADER_GROUP = table_header_group, // TODO: derprecated, remove.
	table_footer_group,
	TABLE_FOOTER_GROUP = table_footer_group, // TODO: derprecated, remove.
	table_row,
	TABLE_ROW = table_row, // TODO: derprecated, remove.
	table_column_group,
	TABLE_COLUMN_GROUP = table_column_group, // TODO: derprecated, remove.
	table_column,
	TABLE_COLUMN = table_column, // TODO: derprecated, remove.
	table_cell,
	TABLE_CELL = table_cell, // TODO: derprecated, remove.
	table_caption,
	TABLE_CAPTION = table_caption, // TODO: derprecated, remove.
	none,
	NONE = none // TODO: derprecated, remove.
};

// TODO: derprecated, remove.
typedef display Display_e;

enum class visibility{
	visible,
	VISIBLE = visible, // TODO: derprecated, remove.
	hidden,
	HIDDEN = hidden, // TODO: derprecated, remove.
	collapse,
	COLLAPSE = collapse // TODO: derprecated, remove.
};

// TODO: deprecated, remove.
typedef visibility Visibility_e;

enum class enable_background{
	accumulate,
	ACCUMULATE = accumulate, // TODO: deprecated, remove.
	new_,
	NEW = new_ // TODO: deprecated, remove.
};

// TODO: deprecated, remove.
typedef enable_background EnableBackground_e;

struct enable_background_property{
	enable_background value;
	real x, y, width, height; // these only make sense when 'value' is NEW.
	
	bool is_rect_specified()const noexcept{
		return this->width >= 0 && this->height >= 0;
	}

	// TODO: deprecated, remove.
	bool isRectSpecified()const noexcept{
		return this->is_rect_specified();
	}
};

// TODO: deprecated, remove.
typedef enable_background_property EnableBackground;

struct style_value{
	enum class type{
		/**
		 * @brief Invalid style value.
		 */
		unknown,
		UNKNOWN = unknown, // TODO: deprecated, remove.

		/**
		 * @brief No special value is used.
		 */
		normal,
		NORMAL = normal, // TODO: deprecated, remove.

		/**
		 * @brief For paint 'none' value.
		 * For paint property (e.g. fill, stroke, etc.) means that color is 'none'
		 */
		none,
		NONE = none, // TODO: deprecated, remove.

		/**
		 * @brief For paint 'currentColor' value.
		 * Means "use 'color' property value".
		 */
		current_color,
		CURRENT_COLOR = current_color, // TODO: deprecated, remove.

		/**
		 * @brief Property 'inherit' value.
		 * Means that property inheritance was explicitly stated using the 'inherit' keyword.
		 */
		inherit,
		INHERIT = inherit, // TODO: deprecated, remove.

		/**
		 * @brief For paint 'url' value.
		 * Means that "str" member holds URL.
		 */
		url,
		URL = url // TODO: deprecated, remove.
	};

	type type_ = type::unknown;

	// TODO: deprecated, remove.
	typedef type Type_e;

	bool is_valid()const noexcept{
		return this->type_ != type::unknown;
	}

	// TODO: deprecated, remove.
	bool isValid()const noexcept{
		return this->is_valid();
	}

	bool is_normal()const noexcept{
		return this->type_ == type::normal;
	}

	// TODO: deprecated, remove.
	bool isNormal()const noexcept{
		return this->is_normal();
	}

	bool is_none()const noexcept{
		return this->type_ == type::none;
	}

	// TODO: deprecated, remove.
	bool isNone()const noexcept{
		return this->is_none();
	}

	bool is_url()const noexcept{
		return this->type_ == type::url;
	}

	// TODO: deprecated, remove.
	bool isUrl()const noexcept{
		return this->is_url();
	}

	union{
		std::uint32_t color;
		real opacity;
		real strokeMiterlimit; // TODO: deprecated, remove.
		real stroke_miterlimit;
		length strokeWidth; // TODO: deprecated, remove.
		length stroke_width;
		svgdom::stroke_line_cap strokeLineCap; // TODO: deprecated, remove.
		svgdom::stroke_line_cap stroke_line_cap;
		svgdom::stroke_line_join strokeLineJoin; // TODO: deprecated, remove.
		svgdom::stroke_line_join stroke_line_join;
		svgdom::fill_rule fillRule; // TODO: deprecated, remove.
		svgdom::fill_rule fill_rule;
		svgdom::color_interpolation colorInterpolationFilters; // TODO: deprecated, remove.
		svgdom::color_interpolation color_interpolation_filters;
		svgdom::display display;
		svgdom::enable_background_property enableBackground; // TODO: deprecated, remove.
		svgdom::enable_background_property enable_background;
		svgdom::visibility visibility;
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
	std::string get_local_id_from_iri()const;

	// TODO: deprecated, remove.
	std::string getLocalIdFromIri()const{
		return this->get_local_id_from_iri();
	}

	static style_value parse_paint(const std::string& str);
	std::string paint_to_string()const;

	// TODO: deprecated, remove.
	static style_value parsePaint(const std::string& str){
		return parse_paint(str);
	}

	// TODO: deprecated, remove.
	std::string paintToString()const{
		return this->paint_to_string();
	}

	static style_value parse_color_interpolation(const std::string& str);

	// TODO: deprecated, remove.
	static style_value parseColorInterpolation(const std::string& str){
		return parse_color_interpolation(str);
	}
	
	static style_value parse_display(const std::string& str);
	std::string display_to_string()const;

	// TODO: deprecated, remove.
	static style_value parseDisplay(const std::string& str){
		return parse_display(str);
	}

	// TODO: deprecated, remove.
	std::string displayToString()const{
		return this->display_to_string();
	}

	static style_value parse_visibility(const std::string& str);
	std::string visibility_to_string()const;

	// TODO: deprecated, remove.
	static style_value parseVisibility(const std::string& str){
		return parse_visibility(str);
	}

	// TODO: deprecated, remove.
	std::string visibilityToString()const{
		return this->visibility_to_string();
	}
	
	static style_value parse_enable_background(const std::string& str);
	std::string enable_background_to_string()const;

	// TODO: deprecated, remove.
	static style_value parseEnableBackground(const std::string& str){
		return parse_enable_background(str);
	}

	// TODO: deprecated, remove.
	std::string enableBackgroundToString()const{
		return this->enable_background_to_string();
	}
	
	std::string color_interpolation_filters_to_string()const;

	// TODO: deprecated, remove.
	std::string colorInterpolationFiltersToString()const{
		return this->color_interpolation_filters_to_string();
	}

	static style_value parse_url(const std::string& str);

	// TODO: deprecated, remove.
	static style_value parseUrl(const std::string& str){
		return parse_url(str);
	}

	/**
	 * @brief get color as RGB.
	 * If this style property represents a color then this method returns the
	 * color as red, green and blue values.
	 * @return RGB structure holding red, green and blue.
	 */
	rgb get_rgb()const;

	// TODO: deprecated, remove.
	rgb getRgb()const{
		return this->get_rgb();
	}
	
	/**
	 * @brief set color from RGB.
	 * If this style property represents a color then this method sets the
	 * color from red, green and blue values.
	 */
	void set_rgb(uint8_t r, uint8_t g, uint8_t b);

	// TODO: deprecated, remove.
	void setRgb(uint8_t r, uint8_t g, uint8_t b){
		this->set_rgb(r, g, b);
	}
};

// TODO: deprecated, remove.
typedef style_value StyleValue;

/**
 * @brief An element which has 'style' attribute or can be styled.
 */
struct styleable : public cssdom::styleable{
	std::map<style_property, style_value> styles;

	std::vector<std::string> classes;

	utki::span<const std::string> get_classes()const override{
		return utki::make_span(this->classes);
	}

	virtual ~styleable()noexcept{}

	const style_value* find_style_property(style_property p)const;

	// TODO: deprecated, remove.
	const style_value* findStyleProperty(style_property p)const{
		return this->find_style_property(p);
	}

	std::string classes_to_string()const;

	std::string styles_to_string()const;

	// TODO: deprecated, remove.
	std::string stylesToString()const{
		return this->styles_to_string();
	}

	static std::string style_value_to_string(style_property p, const style_value& v);

	static decltype(styles) parse(const std::string& str);

	static style_value parse_style_property_value(style_property type, const std::string& str);

	// TODO: deprecated, remove.
	static style_value parseStylePropertyValue(style_property type, const std::string& str){
		return parse_style_property_value(type, str);
	}

	static bool is_inherited(style_property p);

	// TODO: deprecated, remove.
	static bool isStylePropertyInherited(style_property p){
		return is_inherited(p);
	}

	static std::string property_to_string(style_property p);
	static style_property string_to_property(std::string str);

	// TODO: deprecated, remove.
	static std::string propertyToString(style_property p){
		return property_to_string(p);
	}

	// TODO: deprecated, remove.
	static style_property stringToProperty(std::string str){
		return string_to_property(str);
	}
};

// TODO: deprecated, remove.
typedef styleable Styleable;

}
