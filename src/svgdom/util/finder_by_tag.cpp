/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

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
