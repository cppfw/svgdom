#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/Finder.hpp"

#include <chrono>

#include <utki/debug.hpp>

#include <papki/FSFile.hpp>

namespace{
std::uint32_t getTicks(){
	return std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}
}

int main(int argc, char** argv){
	auto loadStart = getTicks();
	
	auto dom = svgdom::load(papki::FSFile("../samples/testdata/back.svg"));
	ASSERT_ALWAYS(dom)
	ASSERT_ALWAYS(dom->children.size() != 0)
	
	TRACE_ALWAYS(<< "SVG loaded in " << float(getTicks() - loadStart) / 1000.0f << " sec." << std::endl)
	
	auto searchStart = getTicks();
	
	class TraverseVisitor : public svgdom::ConstVisitor{
	public:
		svgdom::Finder finder;
		
		TraverseVisitor(const svgdom::Element& root) : finder(root){}
		
		void visit(const svgdom::UseElement& e)override{
			ASSERT_INFO_ALWAYS(this->finder.findById(e.getLocalIdFromIri()), "element not found for id = " << e.getLocalIdFromIri());
		}
		
		void visit(const svgdom::SvgElement& e)override{this->relayAccept(e);}
		void visit(const svgdom::DefsElement& e)override{this->relayAccept(e);}
		void visit(const svgdom::RadialGradientElement& e)override{this->relayAccept(e);}
		void visit(const svgdom::LinearGradientElement& e)override{this->relayAccept(e);}
		void visit(const svgdom::GElement& e)override{this->relayAccept(e);}
	} visitor(*dom);
	
	dom->accept(visitor);
	
	auto searchDuration = getTicks() - searchStart;
	
	ASSERT_INFO_ALWAYS(searchDuration < 5000, "search duration was longer than 5 seconds, actual duration = " << searchDuration)
	
	TRACE_ALWAYS(<< "SVG searched in " << float(searchDuration) / 1000.0f << " sec." << std::endl)
	
	ASSERT_INFO_ALWAYS(visitor.finder.cacheSize() == 17763, "visitor.finder.cacheSize() = " << visitor.finder.cacheSize())
}
