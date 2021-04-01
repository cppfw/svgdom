#include "style_stack_cache.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
public:
	std::unordered_map<std::string, style_stack> cache;
	
	style_stack style_stack;

	void add_to_cache(const svgdom::element& e){
		if(!e.id.empty()){
			this->cache.insert(std::make_pair(e.id, this->style_stack));
		}
	}
	
	void visit_container(const svgdom::element& e, const svgdom::container& c, const svgdom::styleable& s){
		style_stack::push push(this->style_stack, s);
		this->add_to_cache(e);
		this->relay_accept(c);
	}
	void visit_element(const svgdom::element& e, const svgdom::styleable& s){
		style_stack::push push(this->style_stack, s);
		this->add_to_cache(e);
	}
	
	void default_visit(const svgdom::element& e)override{
		this->add_to_cache(e);
	}
	
	void visit(const svgdom::g_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::symbol_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::svg_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::radial_gradient_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::linear_gradient_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::defs_element& e) override{
		this->visit_container(e, e, e);
	}
	void visit(const svgdom::filter_element& e) override{
		this->visit_container(e, e, e);
	}
	
	void visit(const svgdom::polyline_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::circle_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::use_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::gradient::stop_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::path_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::rect_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::line_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::ellipse_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const svgdom::polygon_element& e) override{
		this->visit_element(e, e);
	}	
	void visit(const svgdom::fe_gaussian_blur_element& e) override{
		this->visit_element(e, e);
	}
	void visit(const image_element& e) override{
		this->visit_element(e, e);
	}
};
}

style_stack_cache::style_stack_cache(const svgdom::element& root) :
	cache([&root](){
		cache_creator cc;

		root.accept(cc);

		return std::move(cc.cache);
	}())
{}

const style_stack* style_stack_cache::find(const std::string& id)const{
	if(id.length() == 0){
		return nullptr;
	}
	
	auto i = this->cache.find(id);
	if(i == this->cache.end()){
		return nullptr;
	}
	
	return &i->second;
}
