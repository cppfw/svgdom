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
#include <utki/Void.hpp>

#include <papki/File.hpp>


#include "config.hpp"


namespace svgdom{

struct Length{
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

	real value;
	EUnit unit;
	
	static Length parse(const std::string& str);
	
	static Length make(real value, EUnit unit);
};

class Renderer;

struct Container;

enum class EStyleProperty{
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

struct Rgb{
	real r, g, b;
};

enum class EStrokeLineCap{
	BUTT,
	ROUND,
	SQUARE
};

struct StylePropertyValue{
	bool effective = true;
	
	union{
		std::uint32_t color;
		real opacity;
		Length length;
		EStrokeLineCap strokeLineCap;
	};
	
	std::string str;
	std::unique_ptr<utki::Void> d;

	
	static StylePropertyValue parsePaint(const std::string& str);
	
	std::string paintToString()const;
	
	std::uint32_t getColor()const;
	
	Rgb getRgb()const;
};

struct Element : public utki::Unique{
	Container* parent;
	
	std::string id;
	
	virtual ~Element()noexcept{}
	
	void attribsToStream(std::ostream& s)const;
	
	virtual void toStream(std::ostream& s, unsigned indent = 0)const = 0;
	
	std::string toString()const;
	
	virtual void render(Renderer& renderer)const{}
	
	const StylePropertyValue* getStyleProperty(EStyleProperty property)const;
	
	virtual Element* findById(const std::string& elementId);
};

struct Container : public Element{
	std::vector<std::unique_ptr<Element>> children;
	
	void childrenToStream(std::ostream& s, unsigned indent)const;
	
	void render(Renderer& renderer)const;
	
	Element* findById(const std::string& elementId) override;
};

struct Referencing{
	Element* ref = nullptr;
	std::string iri;
	
	void attribsToStream(std::ostream& s)const;
};


struct Transformable{
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
	std::vector<Transformation> transformations;
	
	void attribsToStream(std::ostream& s)const;
	
	static decltype(Transformable::transformations) parse(const std::string& str);
};


struct Styleable{
	std::map<EStyleProperty, StylePropertyValue> styles;
	
	void attribsToStream(std::ostream& s)const;
	
	static decltype(styles) parse(const std::string& str);
	
	static std::string propertyToString(EStyleProperty p);
	static EStyleProperty stringToProperty(std::string str);
};

struct GElement : public Container, public Transformable, public Styleable{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	void render(Renderer& renderer)const override;
};

struct DefsElement : public Container, public Transformable, public Styleable{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
};

struct Rectangle{
	Length x = Length::make(0, Length::EUnit::PERCENT);
	Length y = Length::make(0, Length::EUnit::PERCENT);
	Length width = Length::make(100, Length::EUnit::PERCENT);
	Length height = Length::make(100, Length::EUnit::PERCENT);
	
	void attribsToStream(std::ostream& s)const;
};

struct SvgElement : public Container, public Rectangle{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	void render(Renderer& renderer) const override;
};

struct Shape : public Element, public Styleable, public Transformable{
	void attribsToStream(std::ostream& s)const;
};

struct PathElement : public Shape{
	struct Step{
		enum class EType{
			UNKNOWN,
			CLOSE,
			MOVE_ABS,
			MOVE_REL,
			LINE_ABS,
			LINE_REL,
			HORIZONTAL_LINE_ABS,
			HORIZONTAL_LINE_REL,
			VERTICAL_LINE_ABS,
			VERTICAL_LINE_REL,
			CUBIC_ABS,
			CUBIC_REL,
			CUBIC_SMOOTH_ABS,
			CUBIC_SMOOTH_REL,
			QUADRATIC_ABS,
			QUADRATIC_REL,
			QUADRATIC_SMOOTH_ABS,
			QUADRATIC_SMOOTH_REL,
			ARC_ABS,
			ARC_REL
		} type;
	
		real x, y;
		
		union{
			real x1;
			real rx;
		};
		
		union{
			real y1;
			real ry;
		};
		
		union{
			real x2;
			real xAxisRotation;
		};
		
		union{
			real y2;
			struct{
				bool largeArc;
				bool sweep;
			} flags;
		};
		
		static EType charToType(char c);
		static char typeToChar(EType t);
	};
	std::vector<Step> path;
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	static decltype(path) parse(const std::string& str);
	
	void render(Renderer& renderer) const override;
};

struct RectElement : public Shape, public Rectangle{
	Length rx = Length::make(0, Length::EUnit::UNKNOWN);
	Length ry = Length::make(0, Length::EUnit::UNKNOWN);
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent) const override;
};

struct Gradient : public Container, public Referencing, public Styleable{
	enum class ESpreadMethod{
		PAD,
		REFLECT,
		REPEAT
	} spreadMethod = ESpreadMethod::PAD;
	
	static std::string spreadMethodToString(ESpreadMethod sm);
	static ESpreadMethod stringToSpreadMethod(const std::string& str);
	
	struct StopElement : public Styleable, public Element{
		real offset;
		
		void toStream(std::ostream& s, unsigned indent)const override;
	};
	
	void attribsToStream(std::ostream& s)const;
};

struct LinearGradientElement : public Gradient{
	Length x1 = Length::make(0, Length::EUnit::PERCENT);
	Length y1 = Length::make(0, Length::EUnit::PERCENT);
	Length x2 = Length::make(100, Length::EUnit::PERCENT);
	Length y2 = Length::make(0, Length::EUnit::PERCENT);
	
	void toStream(std::ostream& s, unsigned indent) const override;
};

struct RadialGradientElement : public Gradient{
	Length cx = Length::make(50, Length::EUnit::PERCENT);
	Length cy = Length::make(50, Length::EUnit::PERCENT);
	Length r = Length::make(50, Length::EUnit::PERCENT);
	Length fx = Length::make(50, Length::EUnit::UNKNOWN);
	Length fy = Length::make(50, Length::EUnit::UNKNOWN);
	
	void toStream(std::ostream& s, unsigned indent) const override;
};


class Renderer{
public:
	virtual void render(const PathElement& e){}
	
	virtual void render(const GElement& e){}
	
	virtual void render(const SvgElement& e){}
	
	virtual ~Renderer()noexcept{}
};



std::unique_ptr<SvgElement> load(const papki::File& f);

}//~namespace


std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
