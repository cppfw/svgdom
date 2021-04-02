#include "elements_by_tag_name_finder.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
private:
	void add_to_cache(const svgdom::element& e){
		if (!e.get_tag().empty()) {
			auto it = cache.find(e.get_tag());

			if (it != cache.end()) {
				it->second.push_back(&e);
			} else {
				std::vector<const element*> elements = {&e};
				cache.insert(std::make_pair(e.get_tag(), std::move(elements)));
			}
		}
	}

public:
	std::unordered_map<std::string, std::vector< const element*>> cache;

	void default_visit(const element& e){
		this->add_to_cache(e);
	}
};
}

elements_by_tag_name_finder::elements_by_tag_name_finder(const svgdom::element& root) :
		cache([&root](){
			cache_creator cc;

			root.accept(cc);

			return std::move(cc.cache);
		}())
{}

const std::vector<const svgdom::element*> elements_by_tag_name_finder::find(const std::string& tag_name)const{
	if(tag_name.length() == 0){
		return {};
	}

	auto i = this->cache.find(tag_name);
	if(i == this->cache.end()){
		return {};
	}

	return i->second;
}

