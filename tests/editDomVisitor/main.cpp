#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/stream_writer.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

// visitor to remove all 'line' elements
class EditingVisitor : public svgdom::visitor{
	std::vector<std::pair<svgdom::container*, decltype(svgdom::container::children)::iterator>> elementsToRemove;
	
	void addToRemove(){
		if(!this->cur_parent()){
			// root element does not have parent, nowhere to remove it from
			return;
		}
		this->elementsToRemove.push_back(std::make_pair(this->cur_parent(), this->cur_iter()));
	}
	
public:
	void visit(svgdom::SvgElement& e) override{
		this->relay_accept(e);
	}
	
	void visit(svgdom::GElement& e) override{
		this->relay_accept(e);
	}
	
	void visit(svgdom::path_element& e) override{
		e.id = "new id for path";
	}

	void visit(svgdom::line_element& e) override{
		this->addToRemove();
	}
	
	void removeLines(){
		for(auto& p : this->elementsToRemove){
			ASSERT_ALWAYS(p.first)
			p.first->children.erase(p.second);
		}
	}
};

int main(int argc, char** argv){
	auto dom = std::make_unique<svgdom::SvgElement>();

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

	dom->children.push_back(std::make_unique<svgdom::path_element>(path));

	dom->children.push_back(std::make_unique<svgdom::line_element>());
	
	{
		auto g = std::make_unique<svgdom::GElement>();
		g->children.push_back(std::make_unique<svgdom::line_element>());
		
		dom->children.push_back(std::move(g));
	}
	
	EditingVisitor visitor;
	
	dom->accept(visitor);
	
	ASSERT_ALWAYS(dom->children.size() == 3)
	ASSERT_ALWAYS(dynamic_cast<svgdom::path_element*>(dom->children.begin()->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::line_element*>((++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get())->children.size() == 1)
	ASSERT_ALWAYS(
			dynamic_cast<svgdom::line_element*>(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get())->children.front().get())
		)
	
	visitor.removeLines();
	
	ASSERT_INFO_ALWAYS(dom->children.size() == 2, "dom->children.size() = " << dom->children.size())
	ASSERT_ALWAYS(dynamic_cast<svgdom::path_element*>(dom->children.begin()->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++dom->children.begin())->get())->children.size() == 0)
}
