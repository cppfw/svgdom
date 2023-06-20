#include <tst/set.hpp>
#include <tst/check.hpp>

#include "../../src/svgdom/elements/structurals.hpp"
#include "../../src/svgdom/elements/shapes.hpp"

tst::set to_string_tests("to_string", [](auto& suite){
	suite.add(
			"path_element_is_converted_to_string",
			[](){
				auto dom = std::make_unique<svgdom::svg_element>();

				svgdom::path_element path;

				svgdom::path_element::step step;

				path.styles[svgdom::style_property::fill] = svgdom::make_style_value(0x42, 0x13, 0xfe);

				step.type_v = svgdom::path_element::step::type::move_abs;
				step.x = 0;
				step.y = 0;
				path.path.push_back(step);

				step.type_v = svgdom::path_element::step::type::line_abs;
				step.x = 0;
				step.y = 300;
				path.path.push_back(step);

				step.type_v = svgdom::path_element::step::type::line_abs;
				step.x = 300;
				step.y = 300;
				path.path.push_back(step);

				step.type_v = svgdom::path_element::step::type::line_abs;
				step.x = 300;
				step.y = 0;
				path.path.push_back(step);

				dom->children.push_back(std::make_unique<svgdom::path_element>(path));

				auto str = dom->to_string();

				LOG([&](auto&o){o << str << '\n';})
				
				tst::check(str.find(R"(xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1")") != std::string::npos, SL);
				
				tst::check(str.find("fill:#4213fe") != std::string::npos, SL);
			}
		);

});
