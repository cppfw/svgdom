#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <map>

#include <utki/Unique.hpp>
#include <utki/Void.hpp>
#include <utki/config.hpp>

#include <papki/File.hpp>


#include "config.hpp"

#if M_OS == M_OS_WINDOWS
#	ifdef IN
#		undef IN
#	endif
#endif

//math.h defines OVERFLOW, undefine it
#ifdef OVERFLOW
#	undef OVERFLOW
#endif

namespace svgdom{

struct Length{
	enum class Unit_e{
		UNKNOWN,
		NUMBER,
		PERCENT,
		EM,
		EX,
		PX,
		CM,
		IN,
		PT,
		PC,
		MM
	};

	real value;
	Unit_e unit;
	
	static Length parse(const std::string& str);
	
	static Length make(real value, Unit_e unit = Unit_e::NUMBER)noexcept;
	
	bool isValid()const noexcept{
		return this->unit != Unit_e::UNKNOWN;
	}
	
	bool isPercent()const noexcept{
		return this->unit == Unit_e::PERCENT;
	}
	
	real toPx(real dpi)const noexcept;
};

class Visitor;

struct Container;
struct Element;

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
	//TODO: move to union?
	std::string str;
	
	static StylePropertyValue parsePaint(const std::string& str);
	
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
 * @brief Base class for all SVG document elements.
 */
struct Element : public utki::Unique{
	//TODO: remove
	Container* parent = nullptr;
	
	std::string id;
	
	virtual ~Element()noexcept{}
	
	//TODO: move all attribsToStream to cpp
	void attribsToStream(std::ostream& s)const;
	
	//TODO: implement via Visitor
	virtual void toStream(std::ostream& s, unsigned indent = 0)const = 0;
	
	//TODO: implement via Visitor
	std::string toString()const;
	
	/**
	 * @brief Accept method for Visitor pattern.
	 * @param visitor - visitor to accept.
	 */
	virtual void accept(Visitor& visitor)const{}
	
	/**
	 * @brief Get style property of the element.
	 * Properties are CSS, so it will get the property from parent elements in case this element
	 * does not define the property.
     * @param property - property to get.
     * @param explicitInherit - the flag is used to indicate that an explicit inheritance is indicated for non-inherited properties.
     * @return pointer to the property value.
	 * @return nullptr in case property was not found.
     */
	const StylePropertyValue* getStyleProperty(StyleProperty_e property, bool explicitInherit = false)const;
	
	/**
	 * @brief Find element by its id.
	 * Searches the elements tree to find an element with indicated id.
     * @param elementId - element id to search for.
     * @return pointer to the found element.
	 * @return nullptr in case the element with given id was not found.
     */
	//TODO: implement via Visitor
	virtual Element* findById(const std::string& elementId);
};

/**
 * @brief An element which can have child elements.
 */
//TODO: do not derive from Element
struct Container : public Element{
	std::vector<std::unique_ptr<Element>> children;
	
	//TODO: move to cpp
	void childrenToStream(std::ostream& s, unsigned indent)const;
	
	/**
	 * @brief Relay accept to children.
	 * @param visitor - visitor to accept.
	 */
	void relayAccept(Visitor& visitor)const;
	
	Element* findById(const std::string& elementId) override;
};

/**
 * @brief an element which can reference another element.
 */
struct Referencing{
	/**
	 * @brief Referenced element.
	 * If the reference is an IRI to an object outside of the SVG document then this variable is nullptr.
	 */
	//TODO: remove
	Element* ref = nullptr;

	/**
	 * @brief IRI reference.
	 * This variable holds the IRI string.
	 */
	std::string iri;
	
	void attribsToStream(std::ostream& s)const;
};

/**
 * @brief An element which has 'transform' attribute or similar.
 */
struct Transformable{
	struct Transformation{
		enum class Type_e{
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
	
	//TODO: move to cpp
	void transformationsToStream(std::ostream& s)const;
	
	//TODO: move to cpp
	static decltype(Transformable::transformations) parse(const std::string& str);
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
	
	static bool isStylePropertyInherited(StyleProperty_e p);
	
	//TODO: move to cpp
	static std::string propertyToString(StyleProperty_e p);
	static StyleProperty_e stringToProperty(std::string str);
};

struct GElement :
		public Container,
		public Transformable,
		public Styleable
{
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	void accept(Visitor& visitor)const override;
};

struct DefsElement :
		public Container,
		public Transformable,
		public Styleable
{
	void toStream(std::ostream& s, unsigned indent = 0)const override;

	void accept(Visitor& visitor)const override;
};

/**
 * @brief A rectangular element.
 */
struct Rectangle{
	Length x = Length::make(0, Length::Unit_e::PERCENT);
	Length y = Length::make(0, Length::Unit_e::PERCENT);
	Length width = Length::make(100, Length::Unit_e::PERCENT);
	Length height = Length::make(100, Length::Unit_e::PERCENT);
	
	void attribsToStream(std::ostream& s)const;
};

struct UseElement :
	public Element,
	public Transformable,
	public Styleable,
	public Referencing,
	public Rectangle
{
	void toStream(std::ostream& s, unsigned indent = 0)const override;

	void accept(Visitor& visitor)const override;
};

//TODO: move to ViewBoxed
enum class PreserveAspectRatio_e{
	NONE,
	X_MIN_Y_MIN,
	X_MID_Y_MIN,
	X_MAX_Y_MIN,
	X_MIN_Y_MID,
	X_MID_Y_MID,
	X_MAX_Y_MID,
	X_MIN_Y_MAX,
	X_MID_Y_MAX,
	X_MAX_Y_MAX
};

/**
 * @brief SVG element which has viewBox attribute.
 * Provides handling of viewBox and preserveAspectRatio attributes.
 */
struct ViewBoxed {
	std::array<real, 4> viewBox = {{-1, -1, -1, -1}};

	struct {
		PreserveAspectRatio_e preserve = PreserveAspectRatio_e::NONE;
		bool defer = false;
		bool slice = false;
	} preserveAspectRatio;

	//TODO: move to cpp
	void parseAndFillPreserveAspectRatio(const std::string& str);

	//TODO: move to cpp
	static decltype(viewBox) parseViewbox(const std::string& str);

	void attribsToStream(std::ostream& s)const;

	bool isViewBoxSpecified()const {
		return this->viewBox[2] >= 0;//width is not negative
	}
};

struct SvgElement :
		public Container,
		public Rectangle,
		public Styleable,
		public ViewBoxed
{
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	void accept(Visitor& visitor) const override;
	
	/**
	 * @brief Get aspect ratio of the element.
	 * @param dpi - dots per inch.
	 * @return aspect ratio of the element.
	 * @return 0 if any of height or weight is specified in percent.
	 */
	real aspectRatio(real dpi)const;
};

struct SymbolElement :
		public Container,
		public Styleable,
		public ViewBoxed
{
	void attribsToStream(std::ostream& s)const;

	void toStream(std::ostream& s, unsigned indent = 0)const override;

	void accept(Visitor& visitor) const override;
};

/**
 * @brief Element representing a geometric shape.
 */
struct Shape :
		public Element,
		public Styleable,
		public Transformable
{
	void attribsToStream(std::ostream& s)const;
};

struct PathElement : public Shape{
	struct Step{
		enum class Type_e{
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
		
		//TODO: move to cpp
		static Type_e charToType(char c);
		static char typeToChar(Type_e t);
	};
	std::vector<Step> path;
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent = 0)const override;
	
	//TODO: move to cpp
	static decltype(path) parse(const std::string& str);
	
	void accept(Visitor& visitor) const override;
};

struct RectElement :
		public Shape,
		public Rectangle
{
	Length rx = Length::make(0, Length::Unit_e::UNKNOWN);
	Length ry = Length::make(0, Length::Unit_e::UNKNOWN);
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct CircleElement : public Shape{
	Length cx = Length::make(0, Length::Unit_e::UNKNOWN);
	Length cy = Length::make(0, Length::Unit_e::UNKNOWN);
	Length r = Length::make(0, Length::Unit_e::UNKNOWN);
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct EllipseElement : public Shape{
	Length cx = Length::make(0, Length::Unit_e::UNKNOWN);
	Length cy = Length::make(0, Length::Unit_e::UNKNOWN);
	Length rx = Length::make(0, Length::Unit_e::UNKNOWN);
	Length ry = Length::make(0, Length::Unit_e::UNKNOWN);
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct LineElement : public Shape{
	Length x1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length x2 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y2 = Length::make(0, Length::Unit_e::UNKNOWN);
	
	void attribsToStream(std::ostream& s)const;
	
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct PolylineShape : public Shape{
	std::vector<std::array<real, 2>> points;
	
	void attribsToStream(std::ostream& s)const;
	
	//TODO: move to cpp
	static decltype(points) parse(const std::string& str);
};


struct PolylineElement : public PolylineShape{
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};

struct PolygonElement : public PolylineShape{
	void toStream(std::ostream& s, unsigned indent) const override;
	
	void accept(Visitor& visitor) const override;
};


/**
 * @brief Common base for gradient elements.
 */
struct Gradient :
		public Container,
		public Referencing,
		public Styleable,
		public Transformable
{
	enum class SpreadMethod_e{
		DEFAULT,
		PAD,
		REFLECT,
		REPEAT
	} spreadMethod = SpreadMethod_e::DEFAULT;
	
	//TODO: remove when svgren is refactored
	SpreadMethod_e getSpreadMethod()const noexcept;
	
	enum class Units_e{
		USER_SPACE_ON_USE,
		OBJECT_BOUNDING_BOX
	} units = Units_e::OBJECT_BOUNDING_BOX;
	
	bool isBoundingBoxUnits()const noexcept{
		return this->units == Units_e::OBJECT_BOUNDING_BOX;
	}
	
	struct StopElement :
			public Element,
			public Styleable
	{
		real offset;
		
		void toStream(std::ostream& s, unsigned indent)const override;
	};
	
	//TODO: remove when svgren is refactored
	const decltype(Container::children)& getStops()const noexcept;
	
	void attribsToStream(std::ostream& s)const;
};

struct LinearGradientElement : public Gradient{
	Length x1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length y1 = Length::make(0, Length::Unit_e::UNKNOWN);
	Length x2 = Length::make(100, Length::Unit_e::UNKNOWN);
	Length y2 = Length::make(0, Length::Unit_e::UNKNOWN);
	
	//TODO: remove when svgren is refactored
	Length getX1()const noexcept;
	Length getY1()const noexcept;
	Length getX2()const noexcept;
	Length getY2()const noexcept;
	
	void toStream(std::ostream& s, unsigned indent) const override;
};

struct RadialGradientElement : public Gradient{
	Length cx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length cy = Length::make(50, Length::Unit_e::UNKNOWN);
	Length r = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fx = Length::make(50, Length::Unit_e::UNKNOWN);
	Length fy = Length::make(50, Length::Unit_e::UNKNOWN);
	
	//TODO: remove when svgren is refactored
	Length getCx()const noexcept;
	Length getCy()const noexcept;
	Length getR()const noexcept;
	
	/**
	 * Can return unit as UNKNOWN in that case fx should coincide with cx.
	 * @return Fx.
	 */
	//TODO: remove when svgren is refactored
	Length getFx()const noexcept;
	
	/**
	 * Can return unit as UNKNOWN in that case fy should coincide with cy.
	 * @return Fy.
	 */
	//TODO: remove when svgren is refactored
	Length getFy()const noexcept;
	
	void toStream(std::ostream& s, unsigned indent) const override;
};

/**
 * @brief Visitor interface.
 * A visitor interface which allows traversing of the SVG element tree.
 * It utilizes the 'visitor' pattern.
 * Each Element-based class can override the 'accept' method which will call
 * corresponding 'visit' method from Visitor. And user can override Visitor's methods
 * to implement their own operation to perform on each SVG element.
 */
class Visitor{
public:
	virtual void visit(const PathElement& e){}
	virtual void visit(const RectElement& e){}
	virtual void visit(const CircleElement& e){}
	virtual void visit(const EllipseElement& e){}
	virtual void visit(const LineElement& e){}
	virtual void visit(const PolylineElement& e){}
	virtual void visit(const PolygonElement& e){}
	virtual void visit(const GElement& e){}
	virtual void visit(const SvgElement& e){}
	virtual void visit(const SymbolElement& e) {}
	virtual void visit(const UseElement& e) {}
	virtual void visit(const DefsElement& e) {}
	
	virtual ~Visitor()noexcept{}
};


/**
 * @brief Load SVG document.
 * Load SVG document from XML file.
 * @param f - file interface to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(const papki::File& f);

/**
 * @brief Load SVG document.
 * Load SVG document from XML stream.
 * @param s - input stream to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(std::istream& s);

/**
 * @brief Load SVG document.
 * Load SVG document from std::string.
 * @param s - input string to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(std::string& s);

}

//TODO: move somewhere
std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
