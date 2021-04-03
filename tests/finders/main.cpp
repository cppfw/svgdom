#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/util/casters.hpp"
#include "../../src/svgdom/util/finder_by_id.hpp"
#include "../../src/svgdom/util/finder_by_class.hpp"
#include "../../src/svgdom/util/finder_by_tag.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

int main(int argc, char** argv){
	auto dom = svgdom::load(papki::fs_file("../samples/testdata/finders.svg"));

	ASSERT_ALWAYS(dom)
	ASSERT_ALWAYS(dom->children.size() != 0)

	// Test element-by-id finder
	{
		svgdom::finder_by_id finder_by_id(*dom);

		ASSERT_INFO_ALWAYS(finder_by_id.size() == 2, "finder_by_id size = " << finder_by_id.size())

		for(const auto& id : {"id1", "id2"}){
			auto element_by_id = finder_by_id.find(id);
			ASSERT_INFO_ALWAYS(element_by_id != nullptr, "element-by-id not found, searched id = " << id)
			ASSERT_ALWAYS(element_by_id->id == id)
		}

		ASSERT_ALWAYS(finder_by_id.find("non_existing_id") == nullptr)
	}

	// Test elements-by-class-name finder
	{
		svgdom::finder_by_class finder_by_class_name(*dom);
		ASSERT_INFO_ALWAYS(finder_by_class_name.size() == 2, "finder_by_class_name size = " << finder_by_class_name.size())

		for(const auto& cls : {"class1", "class2"}){
			auto elements_by_class_name = finder_by_class_name.find(cls);
			ASSERT_INFO_ALWAYS(!elements_by_class_name.empty(), "elements-by-class not found, searched class = " << cls)
			ASSERT_ALWAYS(elements_by_class_name.size() == 1)

			// check that found element is styleable and it has the searched class
			auto s = svgdom::cast_to_styleable(elements_by_class_name[0]);
			ASSERT_ALWAYS(s)
			auto& c = s->classes;
			ASSERT_ALWAYS(std::find(c.begin(), c.end(), cls) != c.end())
		}

		ASSERT_ALWAYS(finder_by_class_name.find("nono_existent_class").empty())
	}

	// Test elements-by-tag-name finder
	{
		svgdom::finder_by_tag finder_by_tag_name(*dom);
		ASSERT_INFO_ALWAYS(finder_by_tag_name.size() == 3, "finder_by_tag_name size = " << finder_by_tag_name.size())

		for(const auto& t : {"circle", "svg", "rect"}){
			auto elements_by_tag_name = finder_by_tag_name.find(t);
			ASSERT_INFO_ALWAYS(!elements_by_tag_name.empty(), "elements-by-tag not found")
			ASSERT_ALWAYS(elements_by_tag_name.size() == 1)
			ASSERT_ALWAYS(elements_by_tag_name[0]->get_tag() == t)
		}

		ASSERT_ALWAYS(finder_by_tag_name.find("non_existent_tag").empty())
	}
}
