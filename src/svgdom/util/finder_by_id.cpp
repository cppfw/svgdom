#include "finder_by_id.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
private:
	void add_to_cache(const svgdom::element& e){
		if(!e.id.empty()){
			this->cache.insert(std::make_pair(e.id, &e));
		}
	}

public:
	std::unordered_map<std::string, const element*> cache;

	void default_visit(const element& e){
		this->add_to_cache(e);
	}
};
}

finder_by_id::finder_by_id(const svgdom::element& root) :
	cache([&root](){
		cache_creator cc;

		root.accept(cc);

		return std::move(cc.cache);
	}())
{}

const svgdom::element* finder_by_id::find(const std::string& id)const noexcept{
	if(id.length() == 0){
		return nullptr;
	}

	auto i = this->cache.find(id);
	if(i == this->cache.end()){
		return nullptr;
	}

	return i->second;
}



