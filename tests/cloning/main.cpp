#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/visitor.hpp"
#include "../../src/svgdom/cloner.hpp"

#include <utki/debug.hpp>

int main(int argc, char** argv){
	std::unique_ptr<svgdom::svg_element> domOriginal = std::make_unique<svgdom::svg_element>();
	
	std::unique_ptr<svgdom::path_element> path = std::make_unique<svgdom::path_element>();

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
	
	ASSERT_ALWAYS(domOriginalStr == domCloneStr)
}
