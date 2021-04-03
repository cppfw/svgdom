#include "finder_by_class.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
private:
	void add_to_cache(const svgdom::element& e, const svgdom::styleable& s){
		for (const auto& class_name : s.classes) {
			auto it = cache.find(class_name);

			if (it != cache.end()) {
				it->second.push_back(&e);
			} else {
				std::vector<const element*> elements = {&e};
				cache.insert(std::make_pair(class_name, std::move(elements)));
			}
		}
	}

public:
	std::unordered_map<std::string, std::vector<const element*>> cache;
	
	void visit_container(const svgdom::element& e, const svgdom::container& c, const svgdom::styleable& s){
		this->add_to_cache(e, s);
		this->relay_accept(c);
	}
	void visit_element(const svgdom::element& e, const svgdom::styleable& s){
		this->add_to_cache(e, s);
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

finder_by_class::finder_by_class(const svgdom::element& root) :
	cache([&root](){
		cache_creator cc;

		root.accept(cc);

		return std::move(cc.cache);
	}())
{}

utki::span<const svgdom::element* const> finder_by_class::find(const std::string& class_name)const noexcept{
	if(class_name.length() == 0){
		return nullptr;
	}

	auto i = this->cache.find(class_name);
	if(i == this->cache.end()){
		return nullptr;
	}

	return utki::make_span(i->second);
}

