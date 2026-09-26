#include <string>
#include <string_view>
#include <variant>

#include <svgdom/dom.hpp>
#include <svgdom/elements/solid_color.hpp>
#include <svgdom/elements/styleable.hpp>
#include <svgdom/util/finder_by_id.hpp>
#include <svgdom/util/finder_by_tag.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

using namespace svgdom;

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const tst::set set("solid_color", [](tst::suite& suite) {
	suite.add("parse_solid_color_element", []() {
		std::string_view svg =
			R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
					<defs>
						<solidColor id="mycolor" solid-color="#a080ff" solid-opacity="0.5"/>
					</defs>
					<circle id="mycircle" fill="url(#mycolor)" cx="50" cy="50" r="40"/>
				</svg>
				)xxx";

		auto dom = svgdom::load(svg);
		tst::check(dom, SL);

		svgdom::finder_by_id finder(*dom);

		// the solidColor element should be found by id and should be castable to solid_color_element
		{
			auto elem = finder.find("mycolor");
			tst::check(elem, SL);
			tst::check_eq(std::string(elem->get_tag()), std::string("solidColor"), SL);

			auto solid = dynamic_cast<const solid_color_element*>(elem);
			tst::check(solid, SL);
			tst::check_eq(std::string(solid->get_id()), std::string("mycolor"), SL);

			// the color and the opacity are stored as presentation attributes
			// check the color: #a080ff => R=0xa0, G=0x80, B=0xff => packed as 0xff80a0
			auto color = solid->get_presentation_attribute(style_property::solid_color);
			tst::check(color, SL);
			tst::check(std::holds_alternative<uint32_t>(*color), SL);
			tst::check_eq(std::get<uint32_t>(*color), 0xff80a0U, SL);

			// check the opacity: 0.5
			auto opacity = solid->get_presentation_attribute(style_property::solid_opacity);
			tst::check(opacity, SL);
			tst::check(std::holds_alternative<real>(*opacity), SL);
			tst::check_eq(std::get<real>(*opacity), real(0.5), SL);
		}

		// the circle referencing the solidColor should hold a url() fill pointing to 'mycolor'
		{
			auto elem = finder.find("mycircle");
			tst::check(elem, SL);

			auto st = dynamic_cast<const styleable*>(elem);
			tst::check(st, SL);

			auto fill = st->get_presentation_attribute(style_property::fill);
			tst::check(fill, SL);
			tst::check(std::holds_alternative<std::string>(*fill), SL);
			tst::check_eq(get_local_id_from_iri(*fill), std::string("mycolor"), SL);
		}
	});

	suite.add("parse_solid_color_different_color_formats", []() {
		auto parse_color = [](std::string_view color_attr) -> uint32_t {
			auto svg = std::string(
						   R"xxx(<svg xmlns="http://www.w3.org/2000/svg">
							<defs>
								<solidColor id="c" solid-color=")xxx"
					   ) +
				std::string(color_attr) +
				std::string(
						   R"xxx("/>
							</defs>
						</svg>)xxx"
				);
			auto dom = svgdom::load(std::string_view(svg));
			tst::check(dom, SL);
			auto finder = svgdom::finder_by_id(*dom);
			auto elem = finder.find("c");
			tst::check(elem, SL);
			auto solid = dynamic_cast<const solid_color_element*>(elem);
			tst::check(solid, SL);
			auto color = solid->get_presentation_attribute(style_property::solid_color);
			tst::check(color, SL);
			tst::check(std::holds_alternative<uint32_t>(*color), SL);
			return std::get<uint32_t>(*color);
		};

		// 6-digit hex: #a080ff => R=0xa0 G=0x80 B=0xff
		tst::check_eq(parse_color("#a080ff"), 0xff80a0U, SL);

		// 3-digit hex: #fff => R=G=B=0xff
		tst::check_eq(parse_color("#fff"), 0xffffffU, SL);

		// rgb() notation: rgb(255, 0, 0) => R=0xff G=0 B=0
		tst::check_eq(parse_color("rgb(255, 0, 0)"), 0x0000ffU, SL);

		// rgb() percentage notation: rgb(0%, 0%, 100%) => R=0 G=0 B=0xff
		tst::check_eq(parse_color("rgb(0%, 0%, 100%)"), 0xff0000U, SL);
	});

	suite.add("parse_solid_color_not_specified", []() {
		std::string_view svg =
			R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg">
					<defs>
						<solidColor id="c"/>
					</defs>
				</svg>
				)xxx";

		auto dom = svgdom::load(svg);
		tst::check(dom, SL);

		auto finder = svgdom::finder_by_id(*dom);
		auto elem = finder.find("c");
		tst::check(elem, SL);
		auto solid = dynamic_cast<const solid_color_element*>(elem);
		tst::check(solid, SL);

		// neither the 'solid-color' nor the 'solid-opacity' are specified
		// => neither presentation attributes nor inline style properties are set
		tst::check(solid->get_presentation_attribute(style_property::solid_color) == nullptr, SL);
		tst::check(solid->get_presentation_attribute(style_property::solid_opacity) == nullptr, SL);
		tst::check(solid->get_style_property(style_property::solid_color) == nullptr, SL);
		tst::check(solid->get_style_property(style_property::solid_opacity) == nullptr, SL);
	});

	suite.add("parse_solid_color_from_style_attribute", []() {
		std::string_view svg =
			R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg">
					<defs>
						<solidColor id="c" style="solid-color: #123456; solid-opacity: 0.25"/>
					</defs>
				</svg>
				)xxx";

		auto dom = svgdom::load(svg);
		tst::check(dom, SL);

		auto finder = svgdom::finder_by_id(*dom);
		auto elem = finder.find("c");
		tst::check(elem, SL);
		auto solid = dynamic_cast<const solid_color_element*>(elem);
		tst::check(solid, SL);

		// the color and the opacity are specified via the inline 'style' attribute
		auto color = solid->get_style_property(style_property::solid_color);
		tst::check(color, SL);
		tst::check(std::holds_alternative<uint32_t>(*color), SL);
		tst::check_eq(std::get<uint32_t>(*color), 0x563412U, SL);

		auto opacity = solid->get_style_property(style_property::solid_opacity);
		tst::check(opacity, SL);
		tst::check(std::holds_alternative<real>(*opacity), SL);
		tst::check_eq(std::get<real>(*opacity), real(0.25), SL);
	});

	suite.add("solid_color_to_string", []() {
		std::string_view svg =
			R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
					<defs>
						<solidColor id="mycolor" solid-color="#a080ff" solid-opacity="0.5"/>
					</defs>
					<circle id="mycircle" fill="url(#mycolor)" cx="50" cy="50" r="40"/>
				</svg>
				)xxx";

		auto dom = svgdom::load(svg);
		tst::check(dom, SL);

		auto str = dom->to_string();

		// the solidColor element should be present in the serialized output
		tst::check(str.find("solidColor") != std::string::npos, SL);
		tst::check(str.find("id=\"mycolor\"") != std::string::npos, SL);
		tst::check(str.find("solid-color=\"#a080ff\"") != std::string::npos, SL);
		tst::check(str.find("solid-opacity=\"0.5\"") != std::string::npos, SL);

		// re-parse the serialized string and make sure the data round-trips
		auto dom2 = svgdom::load(std::string_view(str));
		tst::check(dom2, SL);

		auto finder = svgdom::finder_by_id(*dom2);
		auto elem = finder.find("mycolor");
		tst::check(elem, SL);
		auto solid = dynamic_cast<const solid_color_element*>(elem);
		tst::check(solid, SL);

		auto color = solid->get_presentation_attribute(style_property::solid_color);
		tst::check(color, SL);
		tst::check_eq(std::get<uint32_t>(*color), 0xff80a0U, SL);

		auto opacity = solid->get_presentation_attribute(style_property::solid_opacity);
		tst::check(opacity, SL);
		tst::check_eq(std::get<real>(*opacity), real(0.5), SL);
	});

	suite.add("solid_color_found_by_tag", []() {
		std::string_view svg =
			R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg">
					<defs>
						<solidColor id="c1" solid-color="red"/>
					</defs>
				</svg>
				)xxx";

		auto dom = svgdom::load(svg);
		tst::check(dom, SL);

		svgdom::finder_by_tag finder(*dom);
		auto tags = finder.find("solidColor");
		tst::check_eq(tags.size(), size_t(1), SL);
		if (tags.size() == 1) {
			auto solid = dynamic_cast<const solid_color_element*>(tags[0]);
			tst::check(solid, SL);
			tst::check_eq(std::string(solid->get_id()), std::string("c1"), SL);
		}
	});
});
} // namespace
