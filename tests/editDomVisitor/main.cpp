#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/stream_writer.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

// visitor to remove all 'line' elements
class EditingVisitor : public svgdom::visitor{
	std::vector<std::pair<svgdom::Container*, decltype(svgdom::Container::children)::iterator>> elementsToRemove;
	
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
	
	void visit(svgdom::PathElement& e) override{
		e.id = "new id for path";
	}

	void visit(svgdom::LineElement& e) override{
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

	dom->children.push_back(std::make_unique<svgdom::PathElement>(path));

	dom->children.push_back(std::make_unique<svgdom::LineElement>());
	
	{
		auto g = std::make_unique<svgdom::GElement>();
		g->children.push_back(std::make_unique<svgdom::LineElement>());
		
		dom->children.push_back(std::move(g));
	}
	
	EditingVisitor visitor;
	
	dom->accept(visitor);
	
	ASSERT_ALWAYS(dom->children.size() == 3)
	ASSERT_ALWAYS(dynamic_cast<svgdom::PathElement*>(dom->children.begin()->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::LineElement*>((++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get())->children.size() == 1)
	ASSERT_ALWAYS(
			dynamic_cast<svgdom::LineElement*>(dynamic_cast<svgdom::GElement*>((++++dom->children.begin())->get())->children.front().get())
		)
	
	visitor.removeLines();
	
	ASSERT_INFO_ALWAYS(dom->children.size() == 2, "dom->children.size() = " << dom->children.size())
	ASSERT_ALWAYS(dynamic_cast<svgdom::PathElement*>(dom->children.begin()->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++dom->children.begin())->get()))
	ASSERT_ALWAYS(dynamic_cast<svgdom::GElement*>((++dom->children.begin())->get())->children.size() == 0)
}
