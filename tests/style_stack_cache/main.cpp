#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/util/style_stack_cache.hpp"
#include "../../src/svgdom/visitor.hpp"

#include <chrono>

#include <utki/debug.hpp>
#include <utki/time.hpp>

#include <papki/fs_file.hpp>

int main(int argc, char** argv){
	auto loadStart = utki::get_ticks_ms();
	
	auto dom = svgdom::load(papki::fs_file("../samples/testdata/back.svg"));

	ASSERT_ALWAYS(dom)
	ASSERT_ALWAYS(dom->children.size() != 0)
	
	TRACE_ALWAYS(<< "SVG loaded in " << float(utki::get_ticks_ms() - loadStart) / 1000.0f << " sec." << std::endl)
	
	auto searchStart = utki::get_ticks_ms();
	
	class TraverseVisitor : public svgdom::const_visitor{
	public:
		svgdom::style_stack_cache style_stack_cache;
		
		TraverseVisitor(const svgdom::element& root) : style_stack_cache(root){}
		
		void visit(const svgdom::use_element& e)override{
			ASSERT_INFO_ALWAYS(this->style_stack_cache.find(e.get_local_id_from_iri()), "element not found for id = " << e.get_local_id_from_iri());
		}
	} visitor(*dom);
	
	dom->accept(visitor);
	
	auto searchDuration = utki::get_ticks_ms() - searchStart;
	
	ASSERT_INFO_ALWAYS(searchDuration < 5000, "search duration was longer than 5 seconds, actual duration = " << searchDuration)
	
	TRACE_ALWAYS(<< "SVG searched in " << float(searchDuration) / 1000.0f << " sec." << std::endl)
	
	ASSERT_INFO_ALWAYS(visitor.style_stack_cache.size() == 17763, "visitor.style_stack_cache.size() = " << visitor.style_stack_cache.size())
}
