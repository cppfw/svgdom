#include "finder.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
public:
	std::unordered_map<std::string, const element*> element_by_id_cache;
	std::unordered_map<std::string, std::list<const element*>> elements_by_class_name_cache;
	std::unordered_map<std::string, std::list< const element*>> elements_by_tag_name_cache;
	std::unordered_map<std::string, style_stack> style_stack_by_id_cache;
	
	style_stack styleStack;

	void add_to_element_by_id_cache(const svgdom::element& e){
		if(!e.id.empty()){
			this->element_by_id_cache.insert(std::make_pair(e.id, &e));
		}
	}

	void add_to_elements_by_class_name_cache(const svgdom::element& e, const svgdom::styleable& s){
		for (const auto& class_name : s.classes) {
			auto it = elements_by_class_name_cache.find(class_name);

			if (it != elements_by_class_name_cache.end()) {
				it->second.push_back(&e);
			} else {
				std::list<const element*> elements = {&e};
				elements_by_class_name_cache.insert(std::make_pair(class_name, elements));
			}
		}
	}

	void add_to_elements_by_tag_name_cache(const svgdom::element& e, const svgdom::styleable& s){
		if (!s.get_tag().empty()) {
			auto it = elements_by_tag_name_cache.find(s.get_tag());

			if (it != elements_by_tag_name_cache.end()) {
				it->second.push_back(&e);
			} else {
				std::list<const element*> elements = {&e};
				elements_by_tag_name_cache.insert(std::make_pair(s.get_tag(), elements));
			}
		}
	}

	void add_to_style_stack_by_id_cache(const svgdom::element& e){
		if(!e.id.empty()){
			this->style_stack_by_id_cache.insert(std::make_pair(e.id, this->styleStack));
		}
	}
	
	void visit_container(const svgdom::element& e, const svgdom::container& c, const svgdom::styleable& s){
		style_stack::push push(this->styleStack, s);
		this->add_to_element_by_id_cache(e);
		this->add_to_elements_by_class_name_cache(e, s);
		this->add_to_elements_by_tag_name_cache(e, s);
		this->add_to_style_stack_by_id_cache(e);
		this->relay_accept(c);
	}
	void visit_element(const svgdom::element& e, const svgdom::styleable& s){
		style_stack::push push(this->styleStack, s);
		this->add_to_element_by_id_cache(e);
		this->add_to_elements_by_class_name_cache(e, s);
		this->add_to_elements_by_tag_name_cache(e, s);
		this->add_to_style_stack_by_id_cache(e);
	}
	
	void default_visit(const svgdom::element& e)override{
		this->add_to_style_stack_by_id_cache(e);
		this->add_to_element_by_id_cache(e);
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

finder::finder(const svgdom::element& root){
	cache_creator cc;
	root.accept(cc);

	element_by_id_cache = std::move(cc.element_by_id_cache);
	elements_by_class_name_cache = std::move(cc.elements_by_class_name_cache);
	elements_by_tag_name_cache = std::move(cc.elements_by_tag_name_cache);
	style_stack_by_id_cache = std::move(cc.style_stack_by_id_cache);
}


const svgdom::element* finder::find_element_by_id(const std::string& id)const{
	if(id.length() == 0){
		return nullptr;
	}

	auto i = this->element_by_id_cache.find(id);
	if(i == this->element_by_id_cache.end()){
		return nullptr;
	}

	return i->second;
}

const std::list<const svgdom::element*> finder::find_elements_by_class_name(const std::string& class_name)const{
	if(class_name.length() == 0){
		return {};
	}

	auto i = this->elements_by_class_name_cache.find(class_name);
	if(i == this->elements_by_class_name_cache.end()){
		return {};
	}

	return i->second;
}

const std::list<const svgdom::element*> finder::find_elements_by_tag_name(const std::string& tag_name)const{
	if(tag_name.length() == 0){
		return {};
	}

	auto i = this->elements_by_tag_name_cache.find(tag_name);
	if(i == this->elements_by_tag_name_cache.end()){
		return {};
	}

	return i->second;
}

const style_stack* finder::find_style_stack_by_id(const std::string& id)const{
	if(id.length() == 0){
		return nullptr;
	}
	
	auto i = this->style_stack_by_id_cache.find(id);
	if(i == this->style_stack_by_id_cache.end()){
		return nullptr;
	}
	
	return &i->second;
}
