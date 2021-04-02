#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/util/element_by_id_finder.hpp"
#include "../../src/svgdom/util/elements_by_class_name_finder.hpp"
#include "../../src/svgdom/util/elements_by_tag_name_finder.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

int main(int argc, char** argv){
	auto dom = svgdom::load(papki::fs_file("../samples/testdata/finders.svg"));

	ASSERT_ALWAYS(dom)
	ASSERT_ALWAYS(dom->children.size() != 0)

	// Test element-by-id finder
	svgdom::element_by_id_finder element_by_id_finder(*dom);

	auto element_by_id = element_by_id_finder.find("id1");

	ASSERT_INFO_ALWAYS(element_by_id != nullptr, "element-by-id not found")

	ASSERT_INFO_ALWAYS(element_by_id_finder.size() == 2, "element_by_id_finder size = " << element_by_id_finder.size())

	// Test elements-by-class-name finder
	svgdom::elements_by_class_name_finder elements_by_class_name_finder(*dom);

	auto elements_by_class_name = elements_by_class_name_finder.find("class1");

	ASSERT_INFO_ALWAYS(elements_by_class_name.empty() == false, "elements-by-class not found")

	ASSERT_INFO_ALWAYS(elements_by_class_name_finder.size() == 2, "elements_by_class_name_finder size = " << elements_by_class_name_finder.size())

	// Test elements-by-tag-name finder
	svgdom::elements_by_tag_name_finder elements_by_tag_name_finder(*dom);

	auto elements_by_tag_name = elements_by_tag_name_finder.find("circle");

	ASSERT_INFO_ALWAYS(elements_by_tag_name.empty() == false, "elements-by-tag not found")

	ASSERT_INFO_ALWAYS(elements_by_tag_name_finder.size() == 3, "elements_by_tag_name_finder size = " << elements_by_tag_name_finder.size())
}
