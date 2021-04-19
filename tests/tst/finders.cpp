#include <tst/set.hpp>
#include <tst/check.hpp>

#include <papki/fs_file.hpp>

#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/util/finder_by_id.hpp"

struct fixture{
	std::unique_ptr<svgdom::svg_element> dom;
	fixture(){
		this->dom = svgdom::load(papki::fs_file("../samples/testdata/finders.svg"));
		tst::check(this->dom != nullptr, SL);
		tst::check(this->dom->children.size() != 0, SL);
	}
};
namespace{
tst::set set("finders", [](auto& suite){
	suite.add("element_by_id_finder",[](){
		fixture f;

		svgdom::finder_by_id finder_by_id(*f.dom);

		tst::check(finder_by_id.size() == 2, [&](auto&o){o << "finder_by_id size = " << finder_by_id.size();}, SL);

		for(const auto& id : {"id1", "id2"}){
			auto element_by_id = finder_by_id.find(id);
			tst::check(element_by_id != nullptr, [&](auto&o){o << "element-by-id not found, searched id = " << id;}, SL);
			tst::check(element_by_id->id == id, SL);
		}

		tst::check(finder_by_id.find("non_existing_id") == nullptr, SL);
	});
});
}
