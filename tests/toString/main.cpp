#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

int main(int argc, char** argv){
	auto dom = std::make_unique<svgdom::svg_element>();

	svgdom::path_element path;

	svgdom::path_element::step step;

	svgdom::style_value style;
	style.type_ = svgdom::style_value::type::normal; 
	style.set_rgb(0x42, 0x13, 0xfe); 
	path.styles[svgdom::style_property::fill] = style;

	step.type_ = svgdom::path_element::step::type::move_abs;
	step.x = 0;
	step.y = 0;
	path.path.push_back(step);

	step.type_ = svgdom::path_element::step::type::line_abs;
	step.x = 0;
	step.y = 300;
	path.path.push_back(step);

	step.type_ = svgdom::path_element::step::type::line_abs;
	step.x = 300;
	step.y = 300;
	path.path.push_back(step);

	step.type_ = svgdom::path_element::step::type::line_abs;
	step.x = 300;
	step.y = 0;
	path.path.push_back(step);

	dom->children.push_back(std::make_unique<svgdom::path_element>(path));

	auto str = dom->to_string();
	
	TRACE_ALWAYS(<< str << std::endl)
	
	ASSERT_ALWAYS(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos)
	
	ASSERT_ALWAYS(str.find("fill:#4213fe") != std::string::npos)
}
