#pragma once

#include "container.hpp"
#include "Transformable.hpp"
#include "Styleable.hpp"
#include "referencing.hpp"
#include "rectangle.hpp"
#include "ViewBoxed.hpp"
#include "aspect_ratioed.hpp"
#include "coordinate_units.hpp"

namespace svgdom{

struct GElement :
		public element,
		public container,
		public Transformable,
		public Styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct DefsElement :
		public element,
		public container,
		public Transformable,
		public Styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct UseElement :
		public element,
		public Transformable,
		public referencing,
		public rectangle,
		public Styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct SvgElement :
		public element,
		public container,
		public rectangle,
		public ViewBoxed,
		public aspect_ratioed,
		public Styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
	
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
		public element,
		public container,
		public ViewBoxed,
		public aspect_ratioed,
		public Styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct MaskElement :
		public element,
		public container,
		public rectangle,
		public Styleable
{
	coordinate_units maskUnits;
	coordinate_units maskContentUnits;
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

}
