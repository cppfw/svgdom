#include <tst/set.hpp>
#include <tst/check.hpp>

#include "../../src/svgdom/visitor.hpp"

// visitor to remove all 'line' elements
class editing_visitor : public svgdom::visitor{
	std::vector<std::pair<svgdom::container*, decltype(svgdom::container::children)::iterator>> elementsToRemove;
	
	void add_to_remove(){
		if(!this->cur_parent()){
			// root element does not have parent, nowhere to remove it from
			return;
		}
		this->elementsToRemove.emplace_back(this->cur_parent(), this->cur_iter());
	}
	
public:
	void visit(svgdom::svg_element& e) override{
		this->relay_accept(e);
	}
	
	void visit(svgdom::g_element& e) override{
		this->relay_accept(e);
	}
	
	void visit(svgdom::path_element& e) override{
		e.id = "new id for path";
	}

	void visit(svgdom::line_element& e) override{
		this->add_to_remove();
	}
	
	void remove_lines(){
		for(auto& p : this->elementsToRemove){
			tst::check(p.first, SL);
			p.first->children.erase(p.second);
		}
	}
};

namespace{
const tst::set set("edit_dom_visitor", [](auto& suite){
	suite.add("basic_test", [](){
		auto dom = std::make_unique<svgdom::svg_element>();

		svgdom::path_element path;

		svgdom::path_element::step step{};

		step.type_v = svgdom::path_element::step::type::move_abs;
		step.x = 0;
		step.y = 0;
		path.path.push_back(step);

		step.type_v = svgdom::path_element::step::type::line_abs;
		step.x = 0;
		step.y = 300;
		path.path.push_back(step);

		dom->children.push_back(std::make_unique<svgdom::path_element>(path));

		dom->children.push_back(std::make_unique<svgdom::line_element>());
		
		{
			auto g = std::make_unique<svgdom::g_element>();
			g->children.push_back(std::make_unique<svgdom::line_element>());
			
			dom->children.push_back(std::move(g));
		}
		
		editing_visitor visitor;
		
		dom->accept(visitor);
		
		tst::check_eq(dom->children.size(), size_t(3), SL);
		tst::check(dynamic_cast<svgdom::path_element*>(dom->children.begin()->get()), SL);
		tst::check(dynamic_cast<svgdom::line_element*>((++dom->children.begin())->get()), SL);
		tst::check(dynamic_cast<svgdom::g_element*>((++++dom->children.begin())->get()), SL);
		tst::check_eq(dynamic_cast<svgdom::g_element*>((++++dom->children.begin())->get())->children.size(), size_t(1), SL);
		tst::check(
				dynamic_cast<svgdom::line_element*>(dynamic_cast<svgdom::g_element*>((++++dom->children.begin())->get())->children.front().get()),
				SL
			);
		
		visitor.remove_lines();
		
		tst::check_eq(dom->children.size(), size_t(2), [&](auto&o){o << "dom->children.size() = " << dom->children.size();}, SL);
		tst::check(dynamic_cast<svgdom::path_element*>(dom->children.begin()->get()), SL);
		tst::check(dynamic_cast<svgdom::g_element*>((++dom->children.begin())->get()), SL);
		tst::check_eq(dynamic_cast<svgdom::g_element*>((++dom->children.begin())->get())->children.size(), size_t(0), SL);
	});
});
}
