#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/time.hpp>
#include <papki/fs_file.hpp>

#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/visitor.hpp"
#include "../../src/svgdom/util/style_stack_cache.hpp"

namespace{
tst::set set("style_stack_cache", [](auto& suite){
	suite.add(
			"basic_test",
			[](){
				auto loadStart = utki::get_ticks_ms();
	
				auto dom = svgdom::load(papki::fs_file("../samples/testdata/back.svg"));

				tst::check(dom != nullptr, SL);
				tst::check(dom->children.size() != 0, SL);
				
				utki::log([&](auto&o){o << "SVG loaded in " << float(utki::get_ticks_ms() - loadStart) / 1000.0f << " sec." << std::endl;});
				
				auto searchStart = utki::get_ticks_ms();
				
				class TraverseVisitor : public svgdom::const_visitor{
				public:
					svgdom::style_stack_cache style_stack_cache;
					
					TraverseVisitor(const svgdom::element& root) : style_stack_cache(root){}
					
					void visit(const svgdom::use_element& e)override{
						tst::check(this->style_stack_cache.find(e.get_local_id_from_iri()), [&](auto&o){o << "element not found for id = " << e.get_local_id_from_iri();}, SL);
					}
				} visitor(*dom);
				
				dom->accept(visitor);
				
				auto searchDuration = utki::get_ticks_ms() - searchStart;
				
				tst::check(searchDuration < 5000, [&](auto&o){o << "search duration was longer than 5 seconds, actual duration = " << searchDuration;}, SL);
				
				utki::log([&](auto&o){o << "SVG searched in " << float(searchDuration) / 1000.0f << " sec." << std::endl;});
				
				tst::check(visitor.style_stack_cache.size() == 17763, [&](auto&o){o << "visitor.style_stack_cache.size() = " << visitor.style_stack_cache.size();}, SL);
			}
		);
});
}
