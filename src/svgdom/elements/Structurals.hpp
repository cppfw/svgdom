#pragma once

#include "Container.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"
#include "Referencing.hpp"
#include "Rectangle.hpp"
#include "ViewBoxed.hpp"
#include "AspectRatioed.hpp"
#include "CoordinateUnits.hpp"

namespace svgdom{

struct GElement :
		public Element,
		public Container,
		public Transformable,
		public Styleable
{
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};

struct DefsElement :
		public Element,
		public Container,
		public Transformable,
		public Styleable
{
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};


struct UseElement :
		public Element,
		public Transformable,
		public Referencing,
		public Rectangle,
		public Styleable
{
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};



struct SvgElement :
		public Element,
		public Container,
		public Rectangle,
		public ViewBoxed,
		public AspectRatioed,
		public Styleable
{
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
	
	/**
	 * @brief Get aspect ratio of the element.
	 * @param dpi - dots per inch.
	 * @return aspect ratio of the element.
	 * @return 0 if any of height or weight is specified in percent.
	 */
	real aspectRatio(real dpi)const;
	
	/**
	 * @brief Get natural dimensions in pixels.
	 * If any of the dimensions cannot be converted to pixels, e.g. because specified in percentage,
	 * then this dimensions will be returned as zero.
	 * @param dpi - dots per inch to use when converting lengths to pixels.
	 * @return Width and height.
	 */
	std::array<real, 2> getDimensions(real dpi)const noexcept;
};

struct SymbolElement :
		public Element,
		public Container,
		public ViewBoxed,
		public AspectRatioed,
		public Styleable
{
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};


struct MaskElement :
		public Element,
		public Container,
		public Rectangle,
		public Styleable
{
	CoordinateUnits_e maskUnits;
	CoordinateUnits_e maskContentUnits;
	
	void accept(Visitor& visitor)override;
	void accept(ConstVisitor& visitor) const override;
};

}
