#include <tst/set.hpp>
#include <tst/check.hpp>

#include <string>
#include <string_view>

#include <svgdom/dom.hpp>
#include <svgdom/util/finder_by_id.hpp>
#include <svgdom/elements/styleable.hpp>

using namespace svgdom;
using namespace std::string_literals;

namespace {
// parse a keyword and make sure it round-trips to the exact same keyword, both through the
// property-specific to_string function and through styleable::style_value_to_string (the
// full serialization switch)
template<typename EnumT>
void check_keyword_roundtrip(
	style_property p,
	std::string_view keyword, //
	style_value (*parse)(std::string_view), //
	std::string_view (*to_string)(const style_value&)
)
{
	auto v = parse(keyword);

	// the parsed value must actually hold the expected enumeration type (and not some fallback)
	tst::check(
		std::holds_alternative<EnumT>(v),
		[&](auto& o) {
			o << "parsed value for '" << styleable::property_to_string(p) << "' does not hold the expected enumeration type" << std::endl;
			o << "keyword = " << keyword << std::endl;
		},
		SL
	);

	// property-specific to_string
	tst::check_eq(std::string(to_string(v)), std::string(keyword), SL);

	// full serialization switch
	tst::check_eq(styleable::style_value_to_string(p, v), std::string(keyword), SL);
}
} // namespace

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const tst::set set("style_properties", [](tst::suite& suite){

	suite.add(
		"keyword_properties_roundtrip",
		[](){
			// color-rendering
			check_keyword_roundtrip<color_rendering>(style_property::color_rendering, "auto", &parse_color_rendering, &color_rendering_to_string);
			check_keyword_roundtrip<color_rendering>(style_property::color_rendering, "optimizeSpeed", &parse_color_rendering, &color_rendering_to_string);
			check_keyword_roundtrip<color_rendering>(style_property::color_rendering, "optimizeQuality", &parse_color_rendering, &color_rendering_to_string);

			// font-stretch
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "normal", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "wider", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "narrower", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "ultra-condensed", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "extra-condensed", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "condensed", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "semi-condensed", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "semi-expanded", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "expanded", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "extra-expanded", &parse_font_stretch, &font_stretch_to_string);
			check_keyword_roundtrip<font_stretch>(style_property::font_stretch, "ultra-expanded", &parse_font_stretch, &font_stretch_to_string);

			// font-style
			check_keyword_roundtrip<font_style>(style_property::font_style, "normal", &parse_font_style, &font_style_to_string);
			check_keyword_roundtrip<font_style>(style_property::font_style, "italic", &parse_font_style, &font_style_to_string);
			check_keyword_roundtrip<font_style>(style_property::font_style, "oblique", &parse_font_style, &font_style_to_string);

			// font-variant
			check_keyword_roundtrip<font_variant>(style_property::font_variant, "normal", &parse_font_variant, &font_variant_to_string);
			check_keyword_roundtrip<font_variant>(style_property::font_variant, "small-caps", &parse_font_variant, &font_variant_to_string);

			// image-rendering
			check_keyword_roundtrip<image_rendering>(style_property::image_rendering, "auto", &parse_image_rendering, &image_rendering_to_string);
			check_keyword_roundtrip<image_rendering>(style_property::image_rendering, "optimizeSpeed", &parse_image_rendering, &image_rendering_to_string);
			check_keyword_roundtrip<image_rendering>(style_property::image_rendering, "optimizeQuality", &parse_image_rendering, &image_rendering_to_string);

			// overflow
			check_keyword_roundtrip<overflow>(style_property::overflow, "visible", &parse_overflow, &overflow_to_string);
			check_keyword_roundtrip<overflow>(style_property::overflow, "hidden", &parse_overflow, &overflow_to_string);
			check_keyword_roundtrip<overflow>(style_property::overflow, "scroll", &parse_overflow, &overflow_to_string);
			check_keyword_roundtrip<overflow>(style_property::overflow, "auto", &parse_overflow, &overflow_to_string);

			// shape-rendering
			check_keyword_roundtrip<shape_rendering>(style_property::shape_rendering, "auto", &parse_shape_rendering, &shape_rendering_to_string);
			check_keyword_roundtrip<shape_rendering>(style_property::shape_rendering, "optimizeSpeed", &parse_shape_rendering, &shape_rendering_to_string);
			check_keyword_roundtrip<shape_rendering>(style_property::shape_rendering, "crispEdges", &parse_shape_rendering, &shape_rendering_to_string);
			check_keyword_roundtrip<shape_rendering>(style_property::shape_rendering, "geometricPrecision", &parse_shape_rendering, &shape_rendering_to_string);

			// text-anchor
			check_keyword_roundtrip<text_anchor>(style_property::text_anchor, "start", &parse_text_anchor, &text_anchor_to_string);
			check_keyword_roundtrip<text_anchor>(style_property::text_anchor, "middle", &parse_text_anchor, &text_anchor_to_string);
			check_keyword_roundtrip<text_anchor>(style_property::text_anchor, "end", &parse_text_anchor, &text_anchor_to_string);

			// text-rendering
			check_keyword_roundtrip<text_rendering>(style_property::text_rendering, "auto", &parse_text_rendering, &text_rendering_to_string);
			check_keyword_roundtrip<text_rendering>(style_property::text_rendering, "optimizeSpeed", &parse_text_rendering, &text_rendering_to_string);
			check_keyword_roundtrip<text_rendering>(style_property::text_rendering, "optimizeLegibility", &parse_text_rendering, &text_rendering_to_string);
			check_keyword_roundtrip<text_rendering>(style_property::text_rendering, "geometricPrecision", &parse_text_rendering, &text_rendering_to_string);
		}
	);

	suite.add(
		"font_weight_roundtrip",
		[](){
			// font-weight is stored as the raw string, so any valid value must round-trip exactly
			const std::vector<std::string_view> values = {
				"normal", "bold", "bolder", "lighter", "100", "200", "300", "400", "500", "600", "700", "800", "900"
			};

			for (auto v : values) {
				auto parsed = styleable::parse_style_property_value(style_property::font_weight, v);
				tst::check(std::holds_alternative<std::string>(parsed), SL);
				tst::check_eq(styleable::style_value_to_string(style_property::font_weight, parsed), std::string(v), SL);
			}
		}
	);

	suite.add(
		"clip_path_roundtrip",
		[]{
			// clip-path references a local clipPath element via a url()
			{
				auto v = styleable::parse_style_property_value(style_property::clip_path, "url(#abc)");
				tst::check(std::holds_alternative<std::string>(v), SL);
				// the stored value is the full IRI (including the leading '#'), same as for fill/mask/marker urls
				tst::check_eq(std::get<std::string>(v), std::string("#abc"), SL);
				tst::check_eq(get_local_id_from_iri(v), std::string("abc"), SL);
				tst::check_eq(styleable::style_value_to_string(style_property::clip_path, v), std::string("url(#abc)"), SL);
			}

			// clip-path: none
			{
				auto v = styleable::parse_style_property_value(style_property::clip_path, "none");
				tst::check(is_none(v), SL);
				tst::check_eq(styleable::style_value_to_string(style_property::clip_path, v), std::string("none"), SL);
			}

			// full round-trip through a parsed style attribute
			{
				auto styles = styleable::parse("clip-path: url(#myClip)");
				tst::check(styles.count(style_property::clip_path) != 0, SL);
				tst::check_eq(
					styleable::style_value_to_string(style_property::clip_path, styles.at(style_property::clip_path)),
					std::string("url(#myClip)"),
					SL
				);
			}
		}
	);

	suite.add(
		"inherit_value_roundtrip",
		[](){
			// the 'inherit' keyword is handled generically (before per-property parsing) and must
			// round-trip for every one of the newly implemented style properties. All of the
			// keyword properties above are inheritable per the SVG spec ('Inherited: yes').
			const style_property props[] = {
				style_property::color_rendering,
				style_property::font_stretch,
				style_property::font_style,
				style_property::font_variant,
				style_property::font_weight,
				style_property::image_rendering,
				style_property::overflow,
				style_property::shape_rendering,
				style_property::text_anchor,
				style_property::text_rendering
			};

			for (auto p : props) {
				auto v = styleable::parse_style_property_value(p, "inherit");
				tst::check(
					is_inherit(v),
					[&](auto& o) {
						o << "property: " << styleable::property_to_string(p) << std::endl;
					},
					SL
				);
				tst::check_eq(
					styleable::style_value_to_string(p, v),
					std::string("inherit"),
					[&](auto& o) {
						o << "property: " << styleable::property_to_string(p) << std::endl;
					},
					SL
				);
			}
		}
	);

	suite.add(
		"parse_full_style_attribute",
		[](){
			auto styles = styleable::parse(
				"color-rendering: optimizeQuality; font-stretch: expanded; font-style: oblique; font-variant: small-caps; "
				"font-weight: bold; image-rendering: optimizeQuality; overflow: visible; shape-rendering: geometricPrecision; "
				"text-anchor: end; text-rendering: optimizeLegibility"
			);

			tst::check_eq(styles.size(), size_t(10), SL);

			tst::check_eq(styleable::style_value_to_string(style_property::color_rendering, styles.at(style_property::color_rendering)), std::string("optimizeQuality"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::font_stretch, styles.at(style_property::font_stretch)), std::string("expanded"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::font_style, styles.at(style_property::font_style)), std::string("oblique"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::font_variant, styles.at(style_property::font_variant)), std::string("small-caps"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::font_weight, styles.at(style_property::font_weight)), std::string("bold"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::image_rendering, styles.at(style_property::image_rendering)), std::string("optimizeQuality"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::overflow, styles.at(style_property::overflow)), std::string("visible"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::shape_rendering, styles.at(style_property::shape_rendering)), std::string("geometricPrecision"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::text_anchor, styles.at(style_property::text_anchor)), std::string("end"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::text_rendering, styles.at(style_property::text_rendering)), std::string("optimizeLegibility"), SL);
		}
	);

	suite.add(
		"svg_roundtrip_via_to_string",
		[](){
			std::string_view svg =
				R"xxx(
				<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
					<rect id="r" style="color-rendering: auto; font-stretch: condensed; font-style: italic; font-variant: normal; font-weight: bold; image-rendering: optimizeSpeed; overflow: hidden; shape-rendering: crispEdges; text-anchor: middle; text-rendering: optimizeSpeed"/>
				</svg>
				)xxx";

			auto dom = svgdom::load(svg);
			tst::check(dom, SL);

			auto str = dom->to_string();

			// all the properties must be serialized with their values (and not be left empty as they used to be)
			const std::string expected[] = {
				"color-rendering:auto",
				"font-stretch:condensed",
				"font-style:italic",
				"font-variant:normal",
				"font-weight:bold",
				"image-rendering:optimizeSpeed",
				"overflow:hidden",
				"shape-rendering:crispEdges",
				"text-anchor:middle",
				"text-rendering:optimizeSpeed"
			};

			for (auto& e : expected) {
				tst::check(str.find(e) != std::string::npos, [&](auto& o){ o << "missing: " << e << "\n" << str; }, SL);
			}

			// re-parse the serialized string and make sure the values are preserved
			auto dom2 = svgdom::load(std::string_view(str));
			tst::check(dom2, SL);

			auto finder = svgdom::finder_by_id(*dom2);
			auto elem = finder.find("r");
			tst::check(elem, SL);

			auto st = dynamic_cast<const styleable*>(elem);
			tst::check(st, SL);

			tst::check_eq(std::string(overflow_to_string(*st->get_style_property(style_property::overflow))), std::string("hidden"), SL);
			tst::check_eq(std::string(text_anchor_to_string(*st->get_style_property(style_property::text_anchor))), std::string("middle"), SL);
			tst::check_eq(std::string(font_style_to_string(*st->get_style_property(style_property::font_style))), std::string("italic"), SL);
			tst::check_eq(std::string(shape_rendering_to_string(*st->get_style_property(style_property::shape_rendering))), std::string("crispEdges"), SL);
			tst::check_eq(styleable::style_value_to_string(style_property::font_weight, *st->get_style_property(style_property::font_weight)), std::string("bold"), SL);
		}
	);

});
}
