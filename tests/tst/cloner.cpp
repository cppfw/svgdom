#include <tst/set.hpp>
#include <tst/check.hpp>

#include "../../src/svgdom/util/cloner.hpp"

namespace{
tst::set set("cloner", [](auto& suite){
	suite.add("basic_test", [](){
		auto domOriginal = std::make_unique<svgdom::svg_element>();
	
		auto path = std::make_unique<svgdom::path_element>();

		svgdom::path_element::step step;

		step.type_ = svgdom::path_element::step::type::move_abs;
		step.x = 0;
		step.y = 0;
		path->path.push_back(step);

		step.type_ = svgdom::path_element::step::type::line_abs;
		step.x = 0;
		step.y = 300;
		path->path.push_back(step);

		step.type_ = svgdom::path_element::step::type::line_abs;
		step.x = 300;
		step.y = 300;
		path->path.push_back(step);

		step.type_ = svgdom::path_element::step::type::line_abs;
		step.x = 300;
		step.y = 0;
		path->path.push_back(step);

		domOriginal->children.push_back(std::move(path));

		svgdom::cloner cloner;
		
		domOriginal->accept(cloner);
		
		std::unique_ptr<svgdom::svg_element> domClone = cloner.get_clone_as<svgdom::svg_element>();

		std::string domOriginalStr = domOriginal->to_string();
		
		std::string domCloneStr = domClone->to_string();
		
		tst::check_eq(domOriginalStr, domCloneStr, SL);
	});
});
}
