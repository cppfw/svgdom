#include "finder.hpp"

#include <utki/debug.hpp>

#include "visitor.hpp"

using namespace svgdom;

namespace{
class CacheCreator : virtual public svgdom::const_visitor{
public:
	std::map<std::string, finder::ElementInfo> cache;
	
	StyleStack styleStack;
	
	void addToCache(const svgdom::element& e){
		if(e.id.length() != 0){
			this->cache.insert(std::make_pair(e.id, finder::ElementInfo(e, this->styleStack)));
		}
	}
	
	void visitContainer(const svgdom::element& e, const svgdom::container& c, const svgdom::Styleable& s){
		StyleStack::Push push(this->styleStack, s);
		this->addToCache(e);
		this->relayAccept(c);
	}
	void visitElement(const svgdom::element& e, const svgdom::Styleable& s){
		StyleStack::Push push(this->styleStack, s);
		this->addToCache(e);
	}
	
	void defaultVisit(const svgdom::element& e) override{
		this->addToCache(e);
	}
	
	void visit(const svgdom::g_element& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::SymbolElement& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::SvgElement& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::radial_gradient_element& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::linear_gradient_element& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::defs_element& e) override{
		this->visitContainer(e, e, e);
	}
	void visit(const svgdom::FilterElement& e) override{
		this->visitContainer(e, e, e);
	}
	
	void visit(const svgdom::polyline_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::circle_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::use_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::gradient::stop_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::path_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::rect_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::line_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::ellipse_element& e) override{
		this->visitElement(e, e);
	}
	void visit(const svgdom::polygon_element& e) override{
		this->visitElement(e, e);
	}	
	void visit(const svgdom::FeGaussianBlurElement& e) override{
		this->visitElement(e, e);
	}
	void visit(const image_element& e) override{
		this->visitElement(e, e);
	}
};
}

finder::finder(const svgdom::element& root) :
		cache([&root](){
			CacheCreator visitor;
	
			root.accept(visitor);
			
			return std::move(visitor.cache);
		}())
{}

const finder::ElementInfo* finder::find_by_id(const std::string& id)const{
	if(id.length() == 0){
		return nullptr;
	}
	
	auto i = this->cache.find(id);
	if(i == this->cache.end()){
		return nullptr;
	}
	
	return &i->second;
}
