#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/Visitor.hpp"

#include <utki/debug.hpp>



int main(int argc, char** argv){
	auto dom = std::make_unique<svgdom::SvgElement>();

	svgdom::PathElement path;

	svgdom::PathElement::Step step;

	svgdom::StyleValue style;
	style.type = svgdom::StyleValue::Type_e::NORMAL; 
	style.setRgb(0x42, 0x13, 0xfe); 
	path.styles[svgdom::StyleProperty_e::FILL] = style;

	step.type = svgdom::PathElement::Step::Type_e::MOVE_ABS;
	step.x = 0;
	step.y = 0;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 0;
	step.y = 300;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 300;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 0;
	path.path.push_back(step);

	dom->children.push_back(std::make_unique<svgdom::PathElement>(path));

	auto str = dom->toString();
	
	TRACE_ALWAYS(<< str << std::endl)
	
	ASSERT_ALWAYS(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos)
	
	ASSERT_ALWAYS(str.find("fill:#4213fe") != std::string::npos)
}
