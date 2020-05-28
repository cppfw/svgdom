#pragma once

#include "container.hpp"
#include "transformable.hpp"
#include "styleable.hpp"
#include "referencing.hpp"
#include "rectangle.hpp"
#include "view_boxed.hpp"
#include "aspect_ratioed.hpp"
#include "coordinate_units.hpp"

namespace svgdom{

struct g_element :
		public element,
		public container,
		public transformable,
		public styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef g_element GElement;

struct defs_element :
		public element,
		public container,
		public transformable,
		public styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef defs_element DefsElement;

struct use_element :
		public element,
		public transformable,
		public referencing,
		public rectangle,
		public styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef use_element UseElement;

struct svg_element :
		public element,
		public container,
		public rectangle,
		public view_boxed,
		public aspect_ratioed,
		public styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
	
	/**
	 * @brief Get aspect ratio of the element.
	 * @param dpi - dots per inch.
	 * @return aspect ratio of the element.
	 * @return 0 if any of height or weight is specified in percent.
	 */
	real aspect_ratio(real dpi)const;

	// TODO: deprecated, remove.
	real aspectRatio(real dpi)const{
		return this->aspect_ratio(dpi);
	}
	
	/**
	 * @brief Get natural dimensions in pixels.
	 * If any of the dimensions cannot be converted to pixels, e.g. because specified in percentage,
	 * then this dimensions will be returned as zero.
	 * @param dpi - dots per inch to use when converting lengths to pixels.
	 * @return Width and height.
	 */
	std::array<real, 2> get_dimensions(real dpi)const noexcept;

	// TODO: deprecated, remove.
	std::array<real, 2> getDimensions(real dpi)const noexcept{
		return this->get_dimensions(dpi);
	}
};

// TODO: deprecated, remove.
typedef svg_element SvgElement;

struct symbol_element :
		public element,
		public container,
		public view_boxed,
		public aspect_ratioed,
		public styleable
{
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef symbol_element SymbolElement;

struct mask_element :
		public element,
		public container,
		public rectangle,
		public styleable
{
	coordinate_units mask_units;

	// TODO: deprecated, remove.
	coordinate_units& maskUnits = mask_units;

	coordinate_units mask_content_units;

	// TODO: deprecated, remove.
	coordinate_units& maskContentUnits = mask_content_units;
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;

	const std::string& get_id()const override{
		return this->id;
	}

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

// TODO: deprecated, remove.
typedef mask_element MaskElement;

}
