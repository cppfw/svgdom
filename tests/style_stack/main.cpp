#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/finder.hpp"

#include <utki/debug.hpp>

#include <papki/span_file.hpp>

namespace{
auto svg = R"qwertyuiop(
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

      ]]>
    </style>

	<g>
    	<circle id="green1" class="myGreen" cx="40" cy="40" r="24"/>
	    <circle id="red1" class="myRed" cx="40" cy="100" r="24"/>
		<circle id="blue1" stroke="blue" cx="40" cy="100" r="24"/>
		<circle id="red2" class="myRed" stroke="blue" cx="40" cy="100" r="24"/>
		<circle id="green2" class="myRed" style="stroke: green" cx="40" cy="100" r="24"/>
		<circle id="green3" class="myRed" stroke="blue" style="stroke: green" cx="40" cy="100" r="24"/>
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
		ASSERT_INFO_ALWAYS(sp, "no stroke style property defined for circle with id=" << e.id)

		std::map<std::string, uint32_t> id_to_expected_stroke_map{
			{"green1", 0x6600},
			{"red1", 0x67},
			{"blue1", svgdom::style_value::parse_paint("blue").color},
			{"red2", 0x67},
			{"green2", svgdom::style_value::parse_paint("green").color},
			{"green3", svgdom::style_value::parse_paint("green").color}
		};
		
		auto i = id_to_expected_stroke_map.find(e.id);
		ASSERT_INFO_ALWAYS(i != id_to_expected_stroke_map.end(), "circle with id=" << e.id <<" not found in expected values map")

		ASSERT_INFO_ALWAYS(sp->color == i->second, "expected stroke=0x" << std::hex << i->second <<", got stroke=0x" << sp->color << " for circle with id=" << e.id)
	}
};
}

int main(int argc, char** argv){
	auto dom = svgdom::load(papki::span_file(utki::make_span(svg)));
	ASSERT_ALWAYS(dom)
	ASSERT_ALWAYS(dom->children.size() != 0)
	
	traverse_visitor v;

	dom->accept(v);
}
