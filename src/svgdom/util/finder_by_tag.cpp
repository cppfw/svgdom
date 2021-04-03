#include "finder_by_tag.hpp"

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

finder_by_tag::finder_by_tag(const svgdom::element& root) :
		cache([&root](){
			cache_creator cc;

			root.accept(cc);

			return std::move(cc.cache);
		}())
{}

utki::span<const svgdom::element* const> finder_by_tag::find(const std::string& tag_name)const noexcept{
	if(tag_name.length() == 0){
		return nullptr;
	}

	auto i = this->cache.find(tag_name);
	if(i == this->cache.end()){
		return nullptr;
	}

	return utki::make_span(i->second);
}
