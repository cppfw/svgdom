/* 
 * File:   dom.hpp
 * Author: ivan
 *
 * Created on October 8, 2015, 3:15 PM
 */

#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <map>

#include <utki/Unique.hpp>
#include <papki/File.hpp>


#include "config.hpp"


namespace svgdom{

enum class EUnit{
	UNKNOWN,
	NUMBER,
	PERCENT,
	EM,
	EX,
	PX,
	CM,
	IN,
	PT,
	PC
};

struct Length{
	real value;
	EUnit unit;
	
	Length(real value = 0, EUnit unit = EUnit::NUMBER) :
			value(value),
			unit(unit)
	{}
	
	static Length parse(const std::string& str);
};

struct Element : public utki::Unique{
	std::string id;
	
	virtual ~Element()noexcept{}
	
	void attribsToStream(std::ostream& s)const;
	
	virtual void toStream(std::ostream& s, unsigned indent = 0)const = 0;
	
	std::string toString()const;
};

struct Container{
	std::vector<std::unique_ptr<Element>> children;
	
	void childrenToStream(std::ostream& s, unsigned indent)const;
};

struct Transformation{
	enum class EType{
		MATRIX,
		TRANSLATE,
		SCALE,
		ROTATE,
		SKEWX,
		SKEWY
	} type;
	union{
		real a;
		real angle;
	};
	union{
		real b;
		real x;
	};
	union{
		real c;
		real y;
	};
	real d, e, f;
};

struct Transformable{
	std::vector<Transformation> transformations;
	
	void attribsToStream(std::ostream& s)const;
	
	static decltype(Transformable::transformations) parse(const std::string& str);
};

enum class EStyleProperty{
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

struct StylePropertyValue{
	std::uint32_t integer;
	std::string str;
};

struct Styleable{
	std::map<EStyleProperty, StylePropertyValue> styles;
	
	void attribsToStream(std::ostream& s)const;
	
	static decltype(styles) parse(const std::string& str);
};

struct GElement : public Element, public Container, public Transformable, public Styleable{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
};

struct Rectangle{
	Length x, y;
	Length width = Length(100, EUnit::PERCENT);
	Length height = Length(100, EUnit::PERCENT);
	
	void attribsToStream(std::ostream& s)const;
};

struct SvgElement : public Element, public Container, public Rectangle{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
};




std::unique_ptr<SvgElement> load(const papki::File& f);

}//~namespace


std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
