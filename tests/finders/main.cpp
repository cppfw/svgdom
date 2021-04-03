#include "../../src/svgdom/dom.hpp"
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
	svgdom::finder_by_id finder_by_id(*dom);

	auto element_by_id = finder_by_id.find("id1");

	ASSERT_INFO_ALWAYS(element_by_id != nullptr, "element-by-id not found")

	ASSERT_INFO_ALWAYS(finder_by_id.size() == 2, "finder_by_id size = " << finder_by_id.size())

	// Test elements-by-class-name finder
	svgdom::finder_by_class finder_by_class_name(*dom);

	auto elements_by_class_name = finder_by_class_name.find("class1");

	ASSERT_INFO_ALWAYS(elements_by_class_name.empty() == false, "elements-by-class not found")

	ASSERT_INFO_ALWAYS(finder_by_class_name.size() == 2, "finder_by_class_name size = " << finder_by_class_name.size())

	// Test elements-by-tag-name finder
	svgdom::finder_by_tag finder_by_tag_name(*dom);

	auto elements_by_tag_name = finder_by_tag_name.find("circle");

	ASSERT_INFO_ALWAYS(elements_by_tag_name.empty() == false, "elements-by-tag not found")

	ASSERT_INFO_ALWAYS(finder_by_tag_name.size() == 3, "finder_by_tag_name size = " << finder_by_tag_name.size())
}
