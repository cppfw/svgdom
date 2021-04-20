#include <tst/set.hpp>
#include <tst/check.hpp>

#include <papki/fs_file.hpp>

#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/util/finder_by_id.hpp"
#include "../../src/svgdom/util/finder_by_class.hpp"
#include "../../src/svgdom/util/finder_by_tag.hpp"
#include "../../src/svgdom/util/casters.hpp"

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
	suite.add("finder_by_id",[](){
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

	suite.add("finder_by_class", [](){
		fixture f;

		svgdom::finder_by_class finder_by_class_name(*f.dom);
		tst::check(finder_by_class_name.size() == 2, [&](auto&o){o << "finder_by_class_name size = " << finder_by_class_name.size();}, SL);

		for(const auto& cls : {"class1", "class2"}){
			auto elements_by_class_name = finder_by_class_name.find(cls);
			tst::check(!elements_by_class_name.empty(), [&](auto&o){o << "elements-by-class not found, searched class = " << cls;}, SL);
			tst::check(elements_by_class_name.size() == 1, SL);

			// check that found element is styleable and it has the searched class
			auto s = svgdom::cast_to_styleable(elements_by_class_name.front());
			tst::check(s, SL);
			auto& c = s->classes;
			tst::check(std::find(c.begin(), c.end(), cls) != c.end(), SL);
		}

		tst::check(finder_by_class_name.find("non_existent_class").empty(), SL);
	});

	suite.add("finder_by_tag", [](){
		fixture f;

		svgdom::finder_by_tag finder_by_tag_name(*f.dom);
		tst::check(finder_by_tag_name.size() == 3, [&](auto&o){o << "finder_by_tag_name size = " << finder_by_tag_name.size();}, SL);

		for(const auto& t : {"circle", "svg", "rect"}){
			auto elements_by_tag_name = finder_by_tag_name.find(t);
			tst::check(!elements_by_tag_name.empty(), [&](auto&o){o << "elements-by-tag not found";}, SL);
			tst::check(elements_by_tag_name.size() == 1, SL);
			tst::check(elements_by_tag_name.front()->get_tag() == t, SL);
		}

		tst::check(finder_by_tag_name.find("non_existent_tag").empty(), SL);
	});
});
}
