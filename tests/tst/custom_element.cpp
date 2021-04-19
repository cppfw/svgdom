#include <tst/set.hpp>
#include <tst/check.hpp>

#include "../../src/svgdom/visitor.hpp"
#include "../../src/svgdom/util/stream_writer.hpp"

struct CustomElement : public svgdom::element{
	void accept(svgdom::const_visitor& visitor) const override;
	void accept(svgdom::visitor& visitor) override;

	static const std::string tag;

	const std::string& get_tag()const override{
		return tag;
	}
};

const std::string CustomElement::tag = "custom_element";

class CustomVisitor : virtual public svgdom::const_visitor{
public:
	using svgdom::const_visitor::visit;
	
	virtual void visit(const CustomElement& e){
		this->default_visit(e);
	}
};

void CustomElement::accept(svgdom::const_visitor& visitor) const{
	if(auto v = dynamic_cast<CustomVisitor*>(&visitor)){
		v->visit(*this);
	}else{
		visitor.default_visit(*this);
	}
}

void CustomElement::accept(svgdom::visitor& visitor){
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

namespace{
tst::set set("custom_element", [](auto& suite){
	suite.add("basic_test", [](){
		auto dom = std::make_unique<svgdom::svg_element>();

		svgdom::path_element path;

		svgdom::path_element::step step;

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

		dom->children.push_back(std::make_unique<CustomElement>());
		
		std::stringstream ss;
		CustomStreamWriter writer(ss);
		
		dom->accept(writer);
		
		auto str = ss.str();
		
		TRACE_ALWAYS(<< str << std::endl)
		
		tst::check(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos, SL);
		tst::check(str.find("<custom customAttrib1=\"value1\" customAttrib2=\"value2\"/>") != std::string::npos, SL);
	});
});
}
