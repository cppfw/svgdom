#pragma once

#include "Container.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"
#include "Referencing.hpp"
#include "Rectangle.hpp"
#include "ViewBoxed.hpp"

namespace svgdom{

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


}
