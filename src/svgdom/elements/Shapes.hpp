#pragma once

#include "Transformable.hpp"
#include "Styleable.hpp"
#include "Element.hpp"
#include "Rectangle.hpp"

namespace svgdom{

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

}
