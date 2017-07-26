#pragma once

#include <memory>
#include <ostream>

#include <utki/Unique.hpp>
#include <utki/Void.hpp>
#include <utki/config.hpp>

#include <papki/File.hpp>

#include "config.hpp"
#include "Length.hpp"
#include "elements/Styleable.hpp"
#include "elements/Element.hpp"
#include "elements/Container.hpp"


namespace svgdom{


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
		
		void accept(Visitor& visitor) const override;
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
	
	void accept(Visitor& visitor) const override;
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
	
	void accept(Visitor& visitor) const override;

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
	virtual void visit(const Gradient::StopElement& e){}
	virtual void visit(const LinearGradientElement& e){}
	virtual void visit(const RadialGradientElement& e){}
	
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
