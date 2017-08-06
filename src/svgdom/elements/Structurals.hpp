#pragma once

#include "Container.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"
#include "Referencing.hpp"
#include "Rectangle.hpp"
#include "ViewBoxed.hpp"

namespace svgdom{

struct GElement :
		public Element,
		public Container,
		public Transformable
{
	void accept(Visitor& visitor)const override;
};

struct DefsElement :
		public Element,
		public Container,
		public Transformable
{
	void accept(Visitor& visitor)const override;
};


struct UseElement :
	public Element,
	public Transformable,
	public Referencing,
	public Rectangle
{
	void accept(Visitor& visitor)const override;
};



struct SvgElement :
		public Element,
		public Container,
		public Rectangle,
		public ViewBoxed
{
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
		public Element,
		public Container,
		public ViewBoxed
{
	void accept(Visitor& visitor) const override;
};


}
