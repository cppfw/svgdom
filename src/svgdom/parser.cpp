/*
The MIT License (MIT)

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#include "parser.hxx"

#include <ratio>
#include <string_view>

#include <papki/span_file.hpp>
#include <utki/debug.hpp>
#include <utki/string.hpp>
#include <utki/util.hpp>

#include "util/casters.hpp"

#include "malformed_svg_error.hpp"
#include "util.hxx"

using namespace svgdom;

namespace {
const std::string svg_namespace = "http://www.w3.org/2000/svg";
const std::string xlink_namespace = "http://www.w3.org/1999/xlink";
} // namespace

namespace {
gradient::spread_method gradient_string_to_spread_method(std::string_view str)
{
	if (str == "pad") {
		return gradient::spread_method::pad;
	} else if (str == "reflect") {
		return gradient::spread_method::reflect;
	} else if (str == "repeat") {
		return gradient::spread_method::repeat;
	}
	return gradient::spread_method::default_method;
}
} // namespace

void parser::push_namespaces()
{
	// parse default namespace
	{
		auto i = this->attributes.find("xmlns");
		if (i != this->attributes.end()) {
			if (i->second == svg_namespace) {
				this->default_namespace_stack.push_back(xml_namespace::svg);
			} else if (i->second == xlink_namespace) {
				this->default_namespace_stack.push_back(xml_namespace::xlink);
			} else {
				this->default_namespace_stack.push_back(xml_namespace::unknown);
			}
		} else {
			if (this->default_namespace_stack.size() == 0) {
				this->default_namespace_stack.push_back(xml_namespace::unknown);
			} else {
				this->default_namespace_stack.push_back(this->default_namespace_stack.back());
			}
		}
	}

	// parse other namespaces
	{
		std::string xmlns = "xmlns:";

		this->namespace_stack.emplace_back();

		for (auto& e : this->attributes) {
			const auto& attr = e.first;

			if (attr.substr(0, xmlns.length()) != xmlns) {
				continue;
			}

			ASSERT(attr.length() >= xmlns.length())
			auto ns_name = attr.substr(xmlns.length(), attr.length() - xmlns.length());

			if (e.second == svg_namespace) {
				this->namespace_stack.back()[ns_name] = xml_namespace::svg;
			} else if (e.second == xlink_namespace) {
				this->namespace_stack.back()[ns_name] = xml_namespace::xlink;
			}
		}

		this->flipped_namespace_stack.push_back(utki::flip_map(this->namespace_stack.back()));
	}
}

void parser::pop_namespaces()
{
	ASSERT(this->namespace_stack.size() != 0)
	this->namespace_stack.pop_back();
	ASSERT(this->default_namespace_stack.size() != 0)
	this->default_namespace_stack.pop_back();
	ASSERT(this->flipped_namespace_stack.size() != 0)
	this->flipped_namespace_stack.pop_back();
}

void parser::parse_element()
{
	auto nsn = this->get_namespace(this->cur_element);
	// TRACE(<< "nsn.name = " << nsn.name << std::endl)
	switch (nsn.ns) {
		case xml_namespace::svg:
			if (nsn.name == svg_element::tag) {
				this->parse_svg_element();
			} else if (nsn.name == symbol_element::tag) {
				this->parse_symbol_element();
			} else if (nsn.name == g_element::tag) {
				this->parse_g_element();
			} else if (nsn.name == defs_element::tag) {
				this->parse_defs_element();
			} else if (nsn.name == use_element::tag) {
				this->parse_use_element();
			} else if (nsn.name == path_element::tag) {
				this->parse_path_element();
			} else if (nsn.name == linear_gradient_element::tag) {
				this->parse_linear_gradient_element();
			} else if (nsn.name == radial_gradient_element::tag) {
				this->parse_radial_gradient_element();
			} else if (nsn.name == gradient::stop_element::tag) {
				this->parse_gradient_stop_element();
			} else if (nsn.name == rect_element::tag) {
				this->parse_rect_element();
			} else if (nsn.name == circle_element::tag) {
				this->parse_circle_element();
			} else if (nsn.name == ellipse_element::tag) {
				this->parse_ellipse_element();
			} else if (nsn.name == line_element::tag) {
				this->parse_line_element();
			} else if (nsn.name == polyline_element::tag) {
				this->parse_polyline_element();
			} else if (nsn.name == polygon_element::tag) {
				this->parse_polygon_element();
			} else if (nsn.name == filter_element::tag) {
				this->parse_filter_element();
			} else if (nsn.name == fe_gaussian_blur_element::tag) {
				this->parse_fe_gaussian_blur_element();
			} else if (nsn.name == fe_color_matrix_element::tag) {
				this->parse_fe_color_matrix_element();
			} else if (nsn.name == fe_blend_element::tag) {
				this->parse_fe_blend_element();
			} else if (nsn.name == fe_composite_element::tag) {
				this->parse_fe_composite_element();
			} else if (nsn.name == image_element::tag) {
				this->parse_image_element();
			} else if (nsn.name == mask_element::tag) {
				this->parse_mask_element();
			} else if (nsn.name == text_element::tag) {
				this->parse_text_element();
			} else if (nsn.name == style_element::tag) {
				this->parse_style_element();
			} else {
				// unknown element, ignore
				break;
			}
			return;
		default:
			// unknown namespace, ignore
			break;
	}
	this->element_stack.push_back(nullptr);
}

parser::xml_namespace parser::find_namespace(const std::string& ns)
{
	for (auto i = this->namespace_stack.rbegin(), e = this->namespace_stack.rend(); i != e; ++i) {
		auto iter = i->find(std::string(ns));
		if (iter == i->end()) {
			continue;
		}
		ASSERT(ns == iter->first)
		return iter->second;
	}
	return xml_namespace::unknown;
}

std::optional<std::string_view> parser::find_flipped_namespace(xml_namespace ns)
{
	for (auto i = this->flipped_namespace_stack.rbegin(), e = this->flipped_namespace_stack.rend(); i != e; ++i) {
		auto iter = i->find(ns);
		if (iter == i->end()) {
			continue;
		}
		ASSERT(ns == iter->first)
		return {iter->second};
	}
	return {};
}

parser::namespace_name_pair parser::get_namespace(const std::string& xml_name)
{
	namespace_name_pair ret;

	auto colon_index = xml_name.find_first_of(':');
	if (colon_index == std::string::npos) {
		ret.ns = this->default_namespace_stack.back();
		ret.name = xml_name;
		return ret;
	}

	ASSERT(xml_name.length() >= colon_index + 1)

	ret.ns = this->find_namespace(xml_name.substr(0, colon_index));
	ret.name = xml_name.substr(colon_index + 1, xml_name.length() - 1 - colon_index);

	return ret;
}

const std::string* parser::find_attribute(const std::string& name)
{
	auto i = this->attributes.find(name);
	if (i != this->attributes.end()) {
		return &i->second;
	}
	return nullptr;
}

const std::string* parser::find_attribute_of_namespace(xml_namespace ns, const std::string& name)
{
	if (this->default_namespace_stack.back() == ns) {
		if (auto a = this->find_attribute(name)) {
			return a;
		}
	}

	if (auto prefix = this->find_flipped_namespace(ns)) {
		auto attr = std::string(prefix.value()) + ":" + name;
		if (auto a = this->find_attribute(attr)) {
			return a;
		}
	}
	return nullptr;
}

void parser::fill_element(element& e)
{
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "id")) {
		e.id = *a;
	}
}

void parser::fill_gradient(gradient& g)
{
	this->fill_element(g);
	this->fill_referencing(g);
	this->fill_styleable(g);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "spreadMethod")) {
		g.spread_method_attribute = gradient_string_to_spread_method(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "gradientTransform")) {
		g.transformations = transformable::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "gradientUnits")) {
		g.units = parse_coordinate_units(*a);
	}
}

void parser::fill_rectangle(rectangle& r, const rectangle& default_values)
{
	r = default_values;

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "x")) {
		r.x = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "y")) {
		r.y = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "width")) {
		r.width = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "height")) {
		r.height = length::parse(*a);
	}
}

void parser::fill_referencing(referencing& e)
{
	auto a = this->find_attribute_of_namespace(
		xml_namespace::xlink, //
		"href"
	);
	if (!a) {
		// In some SVG documents the svg namespace is used instead of xlink, though this is against SVG spec we allow to do so.
		a = this->find_attribute_of_namespace(
			xml_namespace::svg, //
			"href"
		);
	}
	if (a) {
		e.iri = *a;
	}
}

void parser::fill_shape(shape& s)
{
	this->fill_element(s);
	this->fill_styleable(s);
	this->fill_transformable(s);
}

void parser::fill_styleable(styleable& s)
{
	ASSERT(s.styles.size() == 0)

	for (auto& a : this->attributes) {
		auto nsn = this->get_namespace(a.first);
		switch (nsn.ns) {
			case xml_namespace::svg:
				if (nsn.name == "style") {
					s.styles = styleable::parse(a.second);
					break;
				} else if (nsn.name == "class") {
					s.classes = utki::split(a.second);
					break;
				}

				// parse style attributes
				{
					style_property type = styleable::string_to_property(nsn.name);
					if (type != style_property::unknown) {
						s.presentation_attributes[type] = styleable::parse_style_property_value(type, a.second);
					}
				}
				break;
			default:
				break;
		}
	}
}

void parser::fill_transformable(transformable& t)
{
	ASSERT(t.transformations.size() == 0)
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "transform")) {
		t.transformations = transformable::parse(*a);
	}
}

void parser::fill_view_boxed(view_boxed& v)
{
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "viewBox")) {
		v.view_box = svg_element::parse_view_box(*a);
	}
}

void parser::fill_text_positioning(text_positioning& p)
{
	// TODO: parse missing attributes
}

void parser::fill_style(style_element& e)
{
	// TODO: parse missing attributes
}

void parser::fill_aspect_ratioed(aspect_ratioed& e)
{
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "preserveAspectRatio")) {
		e.preserve_aspect_ratio.parse(*a);
	}
}

void parser::add_element(std::unique_ptr<element> e)
{
	ASSERT(e)

	auto elem = e.get();

	if (this->element_stack.empty()) {
		if (this->svg) {
			throw malformed_svg_error("more than one root element found in the SVG document");
		}

		element_caster<svg_element> c;
		e->accept(c);
		if (!c.pointer) {
			throw malformed_svg_error("first element of the SVG document is not an 'svg' element");
		}

		[[maybe_unused]] auto ptr = e.release();
		// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
		this->svg = std::unique_ptr<svg_element>(c.pointer);
	} else {
		container_caster c;
		auto parent = this->element_stack.back();
		if (parent) {
			parent->accept(c);
		}
		if (c.pointer) {
			c.pointer->children.push_back(std::move(e));
		} else {
			elem = nullptr;
		}
	}
	this->element_stack.push_back(elem);
}

void parser::parse_circle_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == circle_element::tag)

	auto ret = std::make_unique<circle_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cx")) {
		ret->cx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cy")) {
		ret->cy = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "r")) {
		ret->r = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_defs_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == defs_element::tag)

	auto ret = std::make_unique<defs_element>();

	this->fill_element(*ret);
	this->fill_transformable(*ret);
	this->fill_styleable(*ret);

	this->add_element(std::move(ret));
}

void parser::parse_mask_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == mask_element::tag)

	auto ret = std::make_unique<mask_element>();

	this->fill_element(*ret);
	this->fill_rectangle(*ret);
	this->fill_styleable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "maskUnits")) {
		ret->mask_units = parse_coordinate_units(*a);
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "maskContentUnits")) {
		ret->mask_content_units = parse_coordinate_units(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_text_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == text_element::tag)

	auto ret = std::make_unique<text_element>();

	this->fill_element(*ret);
	this->fill_styleable(*ret);
	this->fill_transformable(*ret);
	this->fill_text_positioning(*ret);

	// TODO: parse missing text element attributes

	this->add_element(std::move(ret));
}

void parser::parse_style_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == style_element::tag)

	auto ret = std::make_unique<style_element>();

	this->fill_element(*ret);
	this->fill_style(*ret);

	// TODO: parse missing style element attributes

	this->add_element(std::move(ret));
}

void parser::parse_ellipse_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == ellipse_element::tag)

	auto ret = std::make_unique<ellipse_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cx")) {
		ret->cx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cy")) {
		ret->cy = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "rx")) {
		ret->rx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "ry")) {
		ret->ry = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_g_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == g_element::tag)

	auto ret = std::make_unique<g_element>();

	this->fill_element(*ret);
	this->fill_transformable(*ret);
	this->fill_styleable(*ret);

	this->add_element(std::move(ret));
}

void parser::parse_gradient_stop_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == gradient::stop_element::tag)

	auto ret = std::make_unique<gradient::stop_element>();

	this->fill_styleable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "offset")) {
		utki::string_parser p(*a);
		ret->offset = p.read_number<real>();
		if (!p.empty() && p.read_char() == '%') {
			ret->offset /= std::centi::den;
		}
	}

	this->add_element(std::move(ret));
}

void parser::parse_line_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == line_element::tag)

	auto ret = std::make_unique<line_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "x1")) {
		ret->x1 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "y1")) {
		ret->y1 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "x2")) {
		ret->x2 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "y2")) {
		ret->y2 = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_filter_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == filter_element::tag)

	auto ret = std::make_unique<filter_element>();

	this->fill_element(*ret);
	this->fill_styleable(*ret);
	this->fill_rectangle(
		*ret,
		rectangle(
			length(filter_element::default_filter_region_x, length_unit::percent),
			length(filter_element::default_filter_region_y, length_unit::percent),
			length(filter_element::default_filter_region_width, length_unit::percent),
			length(filter_element::default_filter_region_height, length_unit::percent)
		)
	);
	this->fill_referencing(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "filterUnits")) {
		ret->filter_units = svgdom::parse_coordinate_units(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "primitiveUnits")) {
		ret->primitive_units = svgdom::parse_coordinate_units(*a);
	}

	this->add_element(std::move(ret));
}

void parser::fill_filter_primitive(filter_primitive& p)
{
	this->fill_element(p);
	this->fill_rectangle(p);
	this->fill_styleable(p);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "result")) {
		p.result = *a;
	}
}

void parser::fill_inputable(inputable& p)
{
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "in")) {
		p.in = *a;
	}
}

void parser::fill_second_inputable(second_inputable& p)
{
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "in2")) {
		p.in2 = *a;
	}
}

void parser::parse_fe_gaussian_blur_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == fe_gaussian_blur_element::tag)

	auto ret = std::make_unique<fe_gaussian_blur_element>();

	this->fill_filter_primitive(*ret);
	this->fill_inputable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "stdDeviation")) {
		ret->std_deviation = parse_number_and_optional_number(*a, {-1, -1});
	}

	this->add_element(std::move(ret));
}

void parser::parse_fe_color_matrix_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == fe_color_matrix_element::tag)

	auto ret = std::make_unique<fe_color_matrix_element>();

	this->fill_filter_primitive(*ret);
	this->fill_inputable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "type")) {
		if (*a == "saturate") {
			ret->type_ = fe_color_matrix_element::type::saturate;
		} else if (*a == "hueRotate") {
			ret->type_ = fe_color_matrix_element::type::hue_rotate;
		} else if (*a == "luminanceToAlpha") {
			ret->type_ = fe_color_matrix_element::type::luminance_to_alpha;
		} else {
			ret->type_ = fe_color_matrix_element::type::matrix; // default value
		}
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "values")) {
		switch (ret->type_) {
			default:
				ASSERT(false) // should never get here, MATRIX should always be the default value
				break;
			case fe_color_matrix_element::type::matrix:
				// 20 values expected
				{
					utki::string_parser p(*a);
					for (auto& v : ret->values) {
						v = p.read_number<real>();
						p.skip_whitespaces_and_comma();
					}
				}
				break;
			case fe_color_matrix_element::type::hue_rotate:
				// fall-through
			case fe_color_matrix_element::type::saturate:
				// one value is expected
				ret->values[0] = utki::string_parser(*a).read_number<real>();
				break;
			case fe_color_matrix_element::type::luminance_to_alpha:
				// no values are expected
				break;
		}
	}

	this->add_element(std::move(ret));
}

void parser::parse_fe_blend_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == fe_blend_element::tag)

	auto ret = std::make_unique<fe_blend_element>();

	this->fill_filter_primitive(*ret);
	this->fill_inputable(*ret);
	this->fill_second_inputable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "mode")) {
		if (*a == "normal") {
			ret->mode_ = fe_blend_element::mode::normal;
		} else if (*a == "multiply") {
			ret->mode_ = fe_blend_element::mode::multiply;
		} else if (*a == "screen") {
			ret->mode_ = fe_blend_element::mode::screen;
		} else if (*a == "darken") {
			ret->mode_ = fe_blend_element::mode::darken;
		} else if (*a == "lighten") {
			ret->mode_ = fe_blend_element::mode::lighten;
		}
	}

	this->add_element(std::move(ret));
}

void parser::parse_fe_composite_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == fe_composite_element::tag)

	auto ret = std::make_unique<fe_composite_element>();

	this->fill_filter_primitive(*ret);
	this->fill_inputable(*ret);
	this->fill_second_inputable(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "operator")) {
		if (*a == "over") {
			ret->operator_attribute = fe_composite_element::operator_type::over;
		} else if (*a == "in") {
			ret->operator_attribute = fe_composite_element::operator_type::in;
		} else if (*a == "out") {
			ret->operator_attribute = fe_composite_element::operator_type::out;
		} else if (*a == "atop") {
			ret->operator_attribute = fe_composite_element::operator_type::atop;
		} else if (*a == "xor") {
			ret->operator_attribute = fe_composite_element::operator_type::xor_operator;
		} else if (*a == "arithmetic") {
			ret->operator_attribute = fe_composite_element::operator_type::arithmetic;
		}
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "k1")) {
		ret->k1 = real(std::strtod(a->c_str(), nullptr));
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "k2")) {
		ret->k2 = real(std::strtod(a->c_str(), nullptr));
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "k3")) {
		ret->k3 = real(std::strtod(a->c_str(), nullptr));
	}

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "k4")) {
		ret->k4 = real(std::strtod(a->c_str(), nullptr));
	}

	this->add_element(std::move(ret));
}

void parser::parse_linear_gradient_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == linear_gradient_element::tag)

	auto ret = std::make_unique<linear_gradient_element>();

	this->fill_gradient(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "x1")) {
		ret->x1 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "y1")) {
		ret->y1 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "x2")) {
		ret->x2 = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "y2")) {
		ret->y2 = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_path_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == path_element::tag)

	auto ret = std::make_unique<path_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "d")) {
		ret->path = path_element::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_polygon_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == polygon_element::tag)

	auto ret = std::make_unique<polygon_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "points")) {
		ret->points = ret->parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_polyline_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == polyline_element::tag)

	auto ret = std::make_unique<polyline_element>();

	this->fill_shape(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "points")) {
		ret->points = ret->parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_radial_gradient_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == radial_gradient_element::tag)

	auto ret = std::make_unique<radial_gradient_element>();

	this->fill_gradient(*ret);

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cx")) {
		ret->cx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "cy")) {
		ret->cy = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "r")) {
		ret->r = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "fx")) {
		ret->fx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "fy")) {
		ret->fy = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_rect_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == rect_element::tag)

	auto ret = std::make_unique<rect_element>();

	this->fill_shape(*ret);
	this->fill_rectangle(*ret, rect_element::rectangle_default_values());

	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "rx")) {
		ret->rx = length::parse(*a);
	}
	if (auto a = this->find_attribute_of_namespace(xml_namespace::svg, "ry")) {
		ret->ry = length::parse(*a);
	}

	this->add_element(std::move(ret));
}

void parser::parse_svg_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == svg_element::tag)

	auto ret = std::make_unique<svg_element>();

	this->fill_element(*ret);
	this->fill_styleable(*ret);
	this->fill_rectangle(*ret);
	this->fill_view_boxed(*ret);
	this->fill_aspect_ratioed(*ret);

	this->add_element(std::move(ret));
}

void parser::parse_image_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == image_element::tag)

	auto ret = std::make_unique<image_element>();

	this->fill_element(*ret);
	this->fill_styleable(*ret);
	this->fill_transformable(*ret);
	this->fill_rectangle(*ret);
	this->fill_referencing(*ret);
	this->fill_aspect_ratioed(*ret);

	this->add_element(std::move(ret));
}

void parser::parse_symbol_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == symbol_element::tag)

	//		TRACE(<< "parse_symbol_element():" << std::endl)

	auto ret = std::make_unique<symbol_element>();

	this->fill_element(*ret);
	this->fill_styleable(*ret);
	this->fill_view_boxed(*ret);
	this->fill_aspect_ratioed(*ret);

	this->add_element(std::move(ret));
}

void parser::parse_use_element()
{
	ASSERT(this->get_namespace(this->cur_element).ns == xml_namespace::svg)
	ASSERT(this->get_namespace(this->cur_element).name == use_element::tag)

	auto ret = std::make_unique<use_element>();

	this->fill_element(*ret);
	this->fill_transformable(*ret);
	this->fill_styleable(*ret);
	this->fill_referencing(*ret);
	this->fill_rectangle(*ret);

	this->add_element(std::move(ret));
}

void parser::on_element_start(utki::span<const char> name)
{
	this->cur_element = utki::make_string(name);
}

void parser::on_element_end(utki::span<const char> name)
{
	this->pop_namespaces();
	this->element_stack.pop_back();
}

void parser::on_attribute_parsed(utki::span<const char> name, utki::span<const char> value)
{
	ASSERT(this->cur_element.length() != 0)
	this->attributes[utki::make_string(name)] = utki::make_string(value);
}

void parser::on_attributes_end(bool is_empty_element)
{
	//	TRACE(<< "this->cur_element = " << this->cur_element << std::endl)
	//	TRACE(<< "this->element_stack.size() = " << this->element_stack.size() << std::endl)
	this->push_namespaces();

	this->parse_element();

	this->attributes.clear();
	this->cur_element.clear();
}

namespace {
class parse_content_visitor : public visitor
{
	const utki::span<const char> content;

public:
	parse_content_visitor(utki::span<const char> content) :
		content(std::move(content))
	{}

	void default_visit(element&, container&) override
	{
		// do nothing
	}

	void visit(style_element& e) override
	{
		e.css.append(
			cssom::read(
				papki::span_file(this->content),
				[](std::string_view name) -> uint32_t {
					return uint32_t(styleable::string_to_property(name));
				},
				[](uint32_t id, std::string_view v) -> std::unique_ptr<cssom::property_value_base> {
					auto sp = style_property(id);
					if (sp == style_property::unknown) {
						return nullptr;
					}
					auto ret = std::make_unique<style_element::css_style_value>();
					ret->value = styleable::parse_style_property_value(sp, v);
					return ret;
				}
			)
		);
	}
};
} // namespace

void parser::on_content_parsed(utki::span<const char> str)
{
	if (this->element_stack.empty() || !this->element_stack.back()) {
		return;
	}

	parse_content_visitor v(str);
	this->element_stack.back()->accept(v);
}

std::unique_ptr<svg_element> parser::get_dom()
{
	if (!this->element_stack.empty()) {
		throw std::invalid_argument("malformed SVG content: unclosed XML tags");
	}
	return std::move(this->svg);
}
