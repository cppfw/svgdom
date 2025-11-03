#include <tst/set.hpp>
#include <tst/check.hpp>

#include <fsif/span_file.hpp>

#include "../../src/svgdom/visitor.hpp"
#include "../../src/svgdom/util/style_stack.hpp"
#include "../../src/svgdom/dom.hpp"

#ifdef assert
#	undef assert
#endif

namespace{
const auto svg = R"qwertyuiop(
<svg xmlns="http://www.w3.org/2000/svg"
    xmlns:xlink="http://www.w3.org/1999/xlink">

    <style type="text/css" >
      <![CDATA[

        circle.myGreen {
           stroke: #006600; fill: #00cc00;
        }
      	circle.myRed {
			stroke: #670000; fill: #cc0000;
    	}
		.cls_yellow_fill {
			fill: yellow
		}
		.cls_blue_stroke {
			stroke: blue
		}

      ]]>
    </style>

	<g>
    	<circle id="green1" class="myGreen" cx="40" cy="40" r="24"/>
	    <circle id="red1" class="myRed" cx="40" cy="100" r="24"/>
		<circle id="blue1" stroke="blue" cx="40" cy="100" r="24"/>
		<circle id="red2" class="myRed" stroke="blue" cx="40" cy="100" r="24"/>
		<circle id="green2" class="myRed" style="stroke: green" cx="40" cy="100" r="24"/>
		<circle id="green3" class="myRed" stroke="blue" style="stroke: green" cx="40" cy="100" r="24"/>
		<rect id="rect_blue_stroke_yellow_fill" class="cls_yellow_fill cls_blue_stroke" x="0" y="0" width="100" height="50"/>
	</g>
	<g class="cls_yellow_fill" stroke="cyan">
		<rect id="g_cyan_stroke_yellow_fill_rect" x="0" y="0" width="100" height="50"/>
	</g>
</svg>
)qwertyuiop";
}

namespace{
class traverse_visitor : public svgdom::const_visitor{
public:
	svgdom::style_stack ss;

	void visit(const svgdom::style_element& e)override{
		this->ss.add_css(e.css);
	}

	void visit(const svgdom::svg_element& e)override{
		svgdom::style_stack::push ss_push(this->ss, e);
		this->relay_accept(e);
	}
	void visit(const svgdom::defs_element& e)override{
		svgdom::style_stack::push ss_push(this->ss, e);
		this->relay_accept(e);
	}
	void visit(const svgdom::g_element& e)override{
		svgdom::style_stack::push ss_push(this->ss, e);
		this->relay_accept(e);
	}

	void visit(const svgdom::circle_element& e)override{
		svgdom::style_stack::push ss_push(this->ss, e);

		auto sp = this->ss.get_style_property(svgdom::style_property::stroke);
		tst::check(sp, [&](auto&o){o << "no stroke style property defined for circle with id=" << e.id;}, SL);

		auto blue = svgdom::parse_paint("blue");
		auto green = svgdom::parse_paint("green");

		std::map<std::string, uint32_t> id_to_expected_stroke_map{
			{"green1", 0x6600},
			{"red1", 0x67},
			{"blue1", *std::get_if<uint32_t>(&blue)},
			{"red2", 0x67},
			{"green2", *std::get_if<uint32_t>(&green)},
			{"green3", *std::get_if<uint32_t>(&green)}
		};
		
		auto i = id_to_expected_stroke_map.find(e.id);
		tst::check(i != id_to_expected_stroke_map.end(), [&](auto&o){o << "circle with id=" << e.id <<" not found in expected values map";}, SL);

		tst::check(*std::get_if<uint32_t>(sp) == i->second, [&](auto&o){o << "expected stroke=0x" << std::hex << i->second <<", got stroke=0x" << *std::get_if<uint32_t>(sp) << " for circle with id=" << e.id;}, SL);
	}

	void visit(const svgdom::rect_element& e)override{
		svgdom::style_stack::push ss_push(this->ss, e);

		auto blue = svgdom::parse_paint("blue");
		auto yellow = svgdom::parse_paint("yellow");
		auto cyan = svgdom::parse_paint("cyan");

		// check stroke
		{
			auto sp = this->ss.get_style_property(svgdom::style_property::stroke);
			tst::check(sp, [&](auto&o){o << "no stroke style property defined for rect with id=" << e.id;}, SL);

			std::map<std::string, uint32_t> id_to_expected_stroke_map{
				{"rect_blue_stroke_yellow_fill", *std::get_if<uint32_t>(&blue)},
				{"g_cyan_stroke_yellow_fill_rect", *std::get_if<uint32_t>(&cyan)}
			};
			
			auto i = id_to_expected_stroke_map.find(e.id);
			tst::check(i != id_to_expected_stroke_map.end(), [&](auto&o){o << "rect with id=" << e.id <<" not found in expected stroke values map";}, SL);

			tst::check(*std::get_if<uint32_t>(sp) == i->second, [&](auto&o){o << "expected stroke=0x" << std::hex << i->second <<", got stroke=0x" << *std::get_if<uint32_t>(sp) << " for rect with id=" << e.id;}, SL);
		}

		// check fill
		{
			auto fp = this->ss.get_style_property(svgdom::style_property::fill);
			tst::check(fp, [&](auto&o){o << "no fill style property defined for rect with id=" << e.id;}, SL);

			std::map<std::string, uint32_t> id_to_expected_fill_map{
				{"rect_blue_stroke_yellow_fill", *std::get_if<uint32_t>(&yellow)},
				{"g_cyan_stroke_yellow_fill_rect", *std::get_if<uint32_t>(&yellow)}
			};
			
			auto i = id_to_expected_fill_map.find(e.id);
			tst::check(i != id_to_expected_fill_map.end(), [&](auto&o){o << "rect with id=" << e.id <<" not found in expected fill values map";}, SL);

			tst::check(*std::get_if<uint32_t>(fp) == i->second, [&](auto&o){o << "expected fill=0x" << std::hex << i->second <<", got fill=0x" << *std::get_if<uint32_t>(fp) << " for rect with id=" << e.id;}, SL);
		}
	}
};
}

namespace{
const tst::set set("style_stack", [](auto& suite){
	suite.add("basic_test", [](){
		auto dom = svgdom::load(fsif::span_file(utki::make_span(svg)));
		utki::assert(dom, SL);
		utki::assert(dom->children.size() != 0, SL);
		
		traverse_visitor v;

		dom->accept(v);
	});
});
}