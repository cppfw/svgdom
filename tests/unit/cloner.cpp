#include <tst/set.hpp>
#include <tst/check.hpp>

#include "../../src/svgdom/util/cloner.hpp"

namespace{
const tst::set set("cloner", [](auto& suite){
	suite.add("basic_test", [](){
		auto dom_original = std::make_unique<svgdom::svg_element>();
	
		auto path = std::make_unique<svgdom::path_element>();

		svgdom::path_element::step step{};

		step.type_v = svgdom::path_element::step::type::move_abs;
		step.x = 0;
		step.y = 0;
		path->path.push_back(step);

		step.type_v = svgdom::path_element::step::type::line_abs;
		step.x = 0;
		step.y = 300;
		path->path.push_back(step);

		step.type_v = svgdom::path_element::step::type::line_abs;
		step.x = 300;
		step.y = 300;
		path->path.push_back(step);

		step.type_v = svgdom::path_element::step::type::line_abs;
		step.x = 300;
		step.y = 0;
		path->path.push_back(step);

		dom_original->children.push_back(std::move(path));

		svgdom::cloner cloner;
		
		dom_original->accept(cloner);
		
		std::unique_ptr<svgdom::svg_element> dom_clone = cloner.get_clone_as<svgdom::svg_element>();

		std::string dom_original_str = dom_original->to_string();
		
		std::string dom_clone_str = dom_clone->to_string();
		
		tst::check_eq(dom_original_str, dom_clone_str, SL);
	});
});
}
