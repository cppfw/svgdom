#include "../../src/svgdom/dom.hpp"

#include <utki/debug.hpp>

#include <papki/FSFile.hpp>


int main(int argc, char** argv){
	auto dom = svgdom::load(papki::FSFile("tiger.svg"));
	
	ASSERT_ALWAYS(dom)
		
	TRACE_ALWAYS(<< dom->toString() << std::endl)
	
	TRACE_ALWAYS(<< "[PASSED]" << std::endl)
}
