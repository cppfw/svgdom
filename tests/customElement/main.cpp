#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/stream_writer.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

struct CustomElement : public svgdom::Element{	
	
	void accept(svgdom::ConstVisitor& visitor) const override;
	void accept(svgdom::Visitor& visitor) override;

};

class CustomVisitor : virtual public svgdom::ConstVisitor{
public:
	using svgdom::ConstVisitor::visit;
	
	virtual void visit(const CustomElement& e){
		this->default_visit(e);
	}
};

void CustomElement::accept(svgdom::ConstVisitor& visitor) const{
	if(auto v = dynamic_cast<CustomVisitor*>(&visitor)){
		v->visit(*this);
	}else{
		visitor.default_visit(*this);
	}
}

void CustomElement::accept(svgdom::Visitor& visitor){
	visitor.default_visit(*this);
}

class CustomStreamWriter :
		public svgdom::stream_writer,
		public CustomVisitor
{
public:
	CustomStreamWriter(std::ostream& s) :
			svgdom::stream_writer(s)
	{}
	
	using svgdom::stream_writer::visit;

	void visit(const CustomElement& e)override{
		this->set_name("custom");
		this->add_attribute("customAttrib1", "value1");
		this->add_attribute("customAttrib2", "value2");
		this->write();
	}
};

int main(int argc, char** argv){
	auto dom = std::make_unique<svgdom::SvgElement>();

	svgdom::PathElement path;

	svgdom::PathElement::Step step;

	step.type_ = svgdom::PathElement::Step::Type_e::MOVE_ABS;
	step.x = 0;
	step.y = 0;
	path.path.push_back(step);

	step.type_ = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 0;
	step.y = 300;
	path.path.push_back(step);

	step.type_ = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 300;
	path.path.push_back(step);

	step.type_ = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 0;
	path.path.push_back(step);

	dom->children.push_back(std::make_unique<svgdom::PathElement>(path));

	dom->children.push_back(std::make_unique<CustomElement>());
	
	std::stringstream ss;
	CustomStreamWriter writer(ss);
	
	dom->accept(writer);
	
	auto str = ss.str();
	
	TRACE_ALWAYS(<< str << std::endl)
	
	ASSERT_ALWAYS(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos)
	ASSERT_ALWAYS(str.find("<custom customAttrib1=\"value1\" customAttrib2=\"value2\"/>") != std::string::npos)
}
