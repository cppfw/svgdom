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

#include "stream_writer.hpp"

#include <papki/vector_file.hpp>
#include <utki/string.hpp>
#include <utki/util.hpp>

#include "../util.hxx"

using namespace svgdom;

void stream_writer::set_name(std::string_view name)
{
	this->name = name;
}

void stream_writer::add_attribute(std::string_view name, const std::string& value)
{
	this->attributes.emplace_back(std::string(name), value);
}

void stream_writer::add_attribute(std::string_view name, const length& value)
{
	std::stringstream ss;
	ss << value;
	this->add_attribute(name, ss.str());
}

void stream_writer::add_attribute(std::string_view name, real value)
{
	std::stringstream ss;
	ss << value;
	this->add_attribute(name, ss.str());
}

void stream_writer::write(const container* children, const std::string& content)
{
	auto ind = this->indent_str();

	auto tag = std::move(this->name);

	this->s << ind << "<" << tag;

	for (auto& a : this->attributes) {
		this->s << " " << a.first << "=\"" << a.second << "\"";
	}

	this->attributes.clear();
	this->name.clear();

	if ((!children || children->children.size() == 0) && content.empty()) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		if (children) {
			this->children_to_stream(*children);
		}
		this->s << content;
		this->s << ind << "</" << tag << ">";
	}
	this->s << std::endl;
}

std::string stream_writer::indent_str()
{
	std::stringstream ss;
	for (unsigned i = 0; i != this->indent; ++i) {
		ss << "\t";
	}
	return ss.str();
}

void stream_writer::children_to_stream(const container& e)
{
	++this->indent;
	utki::scope_exit scope_exit([this]() {
		--this->indent;
	});
	for (auto& c : e.children) {
		c->accept(*this);
	}
}

void stream_writer::add_element_attributes(const element& e)
{
	if (e.id.length() != 0) {
		this->add_attribute("id", e.id);
	}
}

void stream_writer::add_transformable_attributes(const transformable& e)
{
	if (e.transformations.size() != 0) {
		this->add_attribute("transform", e.transformations_to_string());
	}
}

void stream_writer::add_styleable_attributes(const styleable& e)
{
	if (!e.styles.empty()) {
		this->add_attribute("style", e.styles_to_string());
	}
	for (auto& s : e.presentation_attributes) {
		auto n = styleable::property_to_string(s.first);
		if (n.empty()) { // unknown property
			continue;
		}
		this->add_attribute(n, styleable::style_value_to_string(s.first, s.second));
	}
	if (!e.classes.empty()) {
		this->add_attribute("class", e.classes_to_string());
	}
}

void stream_writer::add_view_boxed_attributes(const view_boxed& e)
{
	if (e.is_view_box_specified()) {
		this->add_attribute("viewBox", e.view_box_to_string());
	}
}

void stream_writer::add_aspect_ratioed_attributes(const aspect_ratioed& e)
{
	if (e.preserve_aspect_ratio.preserve != aspect_ratioed::aspect_ratio_preservation::none ||
		e.preserve_aspect_ratio.defer || e.preserve_aspect_ratio.slice)
	{
		this->add_attribute("preserveAspectRatio", e.preserve_aspect_ratio.to_string());
	}
}

void stream_writer::add_rectangle_attributes(const rectangle& e, const rectangle& default_values)
{
	if (e.is_x_specified() && e.x != default_values.x) {
		this->add_attribute("x", e.x);
	}

	if (e.is_y_specified() && e.y != default_values.y) {
		this->add_attribute("y", e.y);
	}

	if (e.is_width_specified() && e.width != default_values.width) {
		this->add_attribute("width", e.width);
	}

	if (e.is_height_specified() && e.height != default_values.height) {
		this->add_attribute("height", e.height);
	}
}

void stream_writer::add_referencing_attributes(const referencing& e)
{
	if (e.iri.length() != 0) {
		this->add_attribute("xlink:href", e.iri);
	}
}

void stream_writer::add_gradient_attributes(const gradient& e)
{
	this->add_element_attributes(e);
	this->add_referencing_attributes(e);
	this->add_styleable_attributes(e);

	if (e.spread_method_attribute != gradient::spread_method::default_method) {
		this->add_attribute("spreadMethod", e.spread_method_to_string());
	}

	if (e.units != coordinate_units::unknown) {
		this->add_attribute("gradientUnits", coordinate_units_to_string(e.units));
	}

	if (e.transformations.size() != 0) {
		this->add_attribute("gradientTransform", e.transformations_to_string());
	}
}

void stream_writer::add_filter_primitive_attributes(const filter_primitive& e)
{
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);

	if (e.result.length() != 0) {
		this->add_attribute("result", e.result);
	}
}

void stream_writer::add_inputable_attributes(const inputable& e)
{
	if (e.in.length() != 0) {
		this->add_attribute("in", e.in);
	}
}

void stream_writer::add_second_inputable_attributes(const second_inputable& e)
{
	if (e.in2.length() != 0) {
		this->add_attribute("in2", e.in2);
	}
}

void stream_writer::add_text_positioning_attributes(const text_positioning& e)
{
	// TODO: add missing attributes
}

void stream_writer::add_shape_attributes(const shape& e)
{
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
}

void stream_writer::visit(const g_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->write(&e);
}

void stream_writer::visit(const svg_element& e)
{
	this->set_name(e.get_tag());

	if (this->indent == 0) { // if outermost "svg" element
		this->add_attribute("xmlns", "http://www.w3.org/2000/svg");
		this->add_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
		this->add_attribute("version", "1.1");
	}

	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_view_boxed_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write(&e);
}

void stream_writer::visit(const image_element& e)
{
	this->set_name(e.get_tag());

	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_transformable_attributes(e);
	this->add_rectangle_attributes(
		e,
		rectangle(
			length(0, length_unit::number),
			length(0, length_unit::number),
			length(0, length_unit::number),
			length(0, length_unit::number)
		)
	);
	this->add_referencing_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write();
}

void stream_writer::visit(const line_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);

	if (e.x1.unit != length_unit::unknown) {
		this->add_attribute("x1", e.x1);
	}

	if (e.y1.unit != length_unit::unknown) {
		this->add_attribute("y1", e.y1);
	}

	if (e.x2.unit != length_unit::unknown) {
		this->add_attribute("x2", e.x2);
	}

	if (e.y2.unit != length_unit::unknown) {
		this->add_attribute("y2", e.y2);
	}

	this->write();
}

void stream_writer::visit(const rect_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);
	this->add_rectangle_attributes(e, rect_element::rectangle_default_values());

	if (e.rx.unit != length_unit::unknown) {
		this->add_attribute("rx", e.rx);
	}

	if (e.ry.unit != length_unit::unknown) {
		this->add_attribute("ry", e.ry);
	}

	this->write();
}

void stream_writer::visit(const ellipse_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);

	if (e.cx.unit != length_unit::unknown) {
		this->add_attribute("cx", e.cx);
	}

	if (e.cy.unit != length_unit::unknown) {
		this->add_attribute("cy", e.cy);
	}

	if (e.rx.unit != length_unit::unknown) {
		this->add_attribute("rx", e.rx);
	}

	if (e.ry.unit != length_unit::unknown) {
		this->add_attribute("ry", e.ry);
	}

	this->write();
}

void stream_writer::visit(const polygon_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);
	if (e.points.size() != 0) {
		this->add_attribute("points", e.points_to_string());
	}
	this->write();
}

void stream_writer::visit(const polyline_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);
	if (e.points.size() != 0) {
		this->add_attribute("points", e.points_to_string());
	}
	this->write();
}

void stream_writer::visit(const circle_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);

	if (e.cx.unit != length_unit::unknown) {
		this->add_attribute("cx", e.cx);
	}

	if (e.cy.unit != length_unit::unknown) {
		this->add_attribute("cy", e.cy);
	}

	if (e.r.unit != length_unit::unknown) {
		this->add_attribute("r", e.r);
	}
	this->write();
}

void stream_writer::visit(const path_element& e)
{
	this->set_name(e.get_tag());
	this->add_shape_attributes(e);
	if (e.path.size() != 0) {
		this->add_attribute("d", e.path_to_string());
	}
	this->write();
}

void stream_writer::visit(const use_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_referencing_attributes(e);
	this->write();
}

void stream_writer::visit(const gradient::stop_element& e)
{
	this->set_name(e.get_tag());
	this->add_attribute("offset", e.offset);
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->write();
}

void stream_writer::visit(const radial_gradient_element& e)
{
	this->set_name(e.get_tag());
	this->add_gradient_attributes(e);
	if (e.cx.unit != length_unit::percent || e.cx.value != real(std::centi::den) / 2) {
		this->add_attribute("cx", e.cx);
	}
	if (e.cy.unit != length_unit::percent || e.cy.value != real(std::centi::den) / 2) {
		this->add_attribute("cy", e.cy);
	}
	if (e.r.unit != length_unit::percent || e.r.value != real(std::centi::den) / 2) {
		this->add_attribute("r", e.r);
	}
	if (e.fx.unit != length_unit::unknown) {
		this->add_attribute("fx", e.fx);
	}
	if (e.fy.unit != length_unit::unknown) {
		this->add_attribute("fy", e.fy);
	}
	this->write(&e);
}

void stream_writer::visit(const linear_gradient_element& e)
{
	this->set_name(e.get_tag());
	this->add_gradient_attributes(e);
	if (e.x1.unit != length_unit::percent || e.x1.value != 0) {
		this->add_attribute("x1", e.x1);
	}
	if (e.y1.unit != length_unit::percent || e.y1.value != 0) {
		this->add_attribute("y1", e.y1);
	}
	if (e.x2.unit != length_unit::percent || e.x2.value != std::centi::den) {
		this->add_attribute("x2", e.x2);
	}
	if (e.y2.unit != length_unit::percent || e.y2.value != 0) {
		this->add_attribute("y2", e.y2);
	}
	this->write(&e);
}

void stream_writer::visit(const defs_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->write(&e);
}

void stream_writer::visit(const mask_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_styleable_attributes(e);

	if (e.mask_units != coordinate_units::object_bounding_box && e.mask_units != coordinate_units::unknown) {
		this->add_attribute("maskUnits", coordinate_units_to_string(e.mask_units));
	}

	if (e.mask_content_units != coordinate_units::user_space_on_use &&
		e.mask_content_units != coordinate_units::unknown)
	{
		this->add_attribute("maskContentUnits", coordinate_units_to_string(e.mask_content_units));
	}

	this->write(&e);
}

namespace {
const auto cdata_open = "<![CDATA[";
const auto cdata_close = "]]>";
} // namespace

void stream_writer::visit(const style_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);

	// TODO: add style element specific attributes

	++this->indent;
	auto ind = this->indent_str();
	--this->indent;

	papki::vector_file fi;
	e.css.write(
		fi,
		[](uint32_t id) -> std::string {
			return std::string(styleable::property_to_string(style_property(id)));
		},
		[](uint32_t id, const cssom::property_value_base& value) -> std::string {
			return styleable::style_value_to_string(
				style_property(id),
				// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
				static_cast<const style_element::css_style_value&>(value).value
			);
		},
		ind
	);

	auto css_vec = fi.reset_data();

	std::stringstream ss;
	if (!css_vec.empty()) {
		ss << ind << cdata_open << std::endl;
		ss << utki::make_string(css_vec);
		ss << ind << cdata_close << std::endl;
	}

	this->write(nullptr, ss.str());
}

void stream_writer::visit(const text_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->add_text_positioning_attributes(e);

	// TODO: add text element attributes

	this->write(&e);
}

void stream_writer::visit(const symbol_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_view_boxed_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write(&e);
}

void stream_writer::visit(const filter_element& e)
{
	this->set_name(e.get_tag());
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(
		e,
		rectangle(
			length(filter_element::default_filter_region_x, length_unit::percent),
			length(filter_element::default_filter_region_y, length_unit::percent),
			length(filter_element::default_filter_region_width, length_unit::percent),
			length(filter_element::default_filter_region_height, length_unit::percent)
		)
	);
	this->add_referencing_attributes(e);

	if (e.filter_units != coordinate_units::unknown && e.filter_units != coordinate_units::object_bounding_box) {
		this->add_attribute("filterUnits", coordinate_units_to_string(e.filter_units));
	}

	if (e.primitive_units != coordinate_units::unknown && e.primitive_units != coordinate_units::user_space_on_use) {
		this->add_attribute("primitiveUnits", coordinate_units_to_string(e.primitive_units));
	}

	this->write(&e);
}

void stream_writer::visit(const fe_gaussian_blur_element& e)
{
	this->set_name(e.get_tag());

	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);

	if (e.is_std_deviation_specified()) {
		this->add_attribute("stdDeviation", number_and_optional_number_to_string(e.std_deviation, -1));
	}
	this->write();
}

void stream_writer::visit(const fe_color_matrix_element& e)
{
	this->set_name(e.get_tag());

	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);

	{
		std::string type_value;

		// write type
		switch (e.type_) {
			default:
			case fe_color_matrix_element::type::matrix:
				// default value is 'matrix', so omit type attribute.
				break;
			case fe_color_matrix_element::type::hue_rotate:
				type_value = "hueRotate";
				break;
			case fe_color_matrix_element::type::saturate:
				type_value = "saturate";
				break;
			case fe_color_matrix_element::type::luminance_to_alpha:
				type_value = "luminanceToAlpha";
				break;
		}
		if (type_value.length() != 0) {
			this->add_attribute("type", type_value);
		}
	}

	{
		std::string values_value;
		switch (e.type_) {
			default:
			case fe_color_matrix_element::type::matrix:
				// write 20 values
				{
					std::stringstream ss;
					bool first_value = true;
					for (const auto& v : e.values) {
						if (!first_value) {
							ss << " ";
						} else {
							first_value = false;
						}
						ss << v;
					}
					values_value = ss.str();
				}
				break;
			case fe_color_matrix_element::type::hue_rotate:
				// fall-through
			case fe_color_matrix_element::type::saturate:
				// write 1 value
				values_value = std::to_string(e.values[0]);
				break;
			case fe_color_matrix_element::type::luminance_to_alpha:
				// 'values' attribute can be omitted, so do nothing
				break;
		}
		if (values_value.length() != 0) {
			this->add_attribute("values", values_value);
		}
	}

	this->write();
}

void stream_writer::visit(const fe_blend_element& e)
{
	this->set_name(e.get_tag());

	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	this->add_second_inputable_attributes(e);

	{
		std::string mode_value;
		switch (e.mode_) {
			default:
			case fe_blend_element::mode::normal:
				// default value, can be omitted
				break;
			case fe_blend_element::mode::multiply:
				mode_value = "multiply";
				break;
			case fe_blend_element::mode::screen:
				mode_value = "screen";
				break;
			case fe_blend_element::mode::darken:
				mode_value = "darken";
				break;
			case fe_blend_element::mode::lighten:
				mode_value = "lighten";
				break;
		}
		this->add_attribute("mode", mode_value);
	}

	this->write();
}

void stream_writer::visit(const fe_composite_element& e)
{
	this->set_name(e.get_tag());

	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	this->add_second_inputable_attributes(e);

	{
		std::string operator_value;
		switch (e.operator_attribute) {
			default:
			case fe_composite_element::operator_type::over:
				// default value, can be omitted
				break;
			case fe_composite_element::operator_type::in:
				operator_value = "in";
				break;
			case fe_composite_element::operator_type::out:
				operator_value = "out";
				break;
			case fe_composite_element::operator_type::atop:
				operator_value = "atop";
				break;
			case fe_composite_element::operator_type::xor_operator:
				operator_value = "xor";
				break;
			case fe_composite_element::operator_type::arithmetic:
				operator_value = "arithmetic";
				break;
		}
		if (operator_value.length() != 0) {
			this->add_attribute("operator", operator_value);
		}
	}

	if (e.k1 != real(0)) {
		this->add_attribute("k1", std::to_string(e.k1));
	}

	if (e.k2 != real(0)) {
		this->add_attribute("k2", std::to_string(e.k2));
	}

	if (e.k3 != real(0)) {
		this->add_attribute("k3", std::to_string(e.k3));
	}

	if (e.k4 != real(0)) {
		this->add_attribute("k4", std::to_string(e.k4));
	}

	this->write();
}
