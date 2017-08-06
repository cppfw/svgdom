#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/StreamWriter.hpp"

#include <utki/debug.hpp>

struct CustomElement : public svgdom::Element{
	std::string str;
	
	CustomElement(std::string&& str) :
			str(std::move(str))
	{}
	
	CustomElement(const std::string& str) :
			CustomElement(std::string(str))
	{}
	
	void accept(svgdom::Visitor& visitor) const override;
};

class CustomStreamWriter : public svgdom::StreamWriter{
public:
	CustomStreamWriter(std::ostream& s) :
			svgdom::StreamWriter(s)
	{}
	
	using svgdom::StreamWriter::visit;

	virtual void visit(const CustomElement& e){
		this->setName("custom");
		this->addAttribute("customAttrib1", "value1");
		this->addAttribute("customAttrib2", "value2");
		this->write();
	}
};

void CustomElement::accept(svgdom::Visitor& visitor) const{
	if(auto v = dynamic_cast<CustomStreamWriter*>(&visitor)){
		v->visit(*this);
	}else{
		visitor.defaultVisit(*this);
	}
}

int main(int argc, char** argv){
	auto dom = utki::makeUnique<svgdom::SvgElement>();

	svgdom::PathElement path;

	svgdom::PathElement::Step step;

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

	dom->children.push_back(utki::makeUnique<svgdom::PathElement>(path));

	dom->children.push_back(utki::makeUnique<CustomElement>("Any string goes here."));
	
	std::stringstream ss;
	CustomStreamWriter writer(ss);
	
	dom->accept(writer);
	
	auto str = ss.str();
	
	TRACE_ALWAYS(<< str << std::endl)
	
	ASSERT_ALWAYS(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos)
	ASSERT_ALWAYS(str.find("<custom customAttrib1=\"value1\" customAttrib2=\"value2\"/>") != std::string::npos)
}
