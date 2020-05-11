#include "../../src/svgdom/dom.hpp"

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
	
	auto buf = papki::FSFile("../samples/testdata/back.svg").loadWholeFileIntoMemory();
	
	TRACE_ALWAYS(<< "SVG loaded in " << float(getTicks() - loadStart) / 1000.0f << " sec." << std::endl)
	
	for(unsigned i = 0; i != 5; ++i){
		auto parseStart = getTicks();
		auto dom = svgdom::load(utki::make_span(buf));
		ASSERT_ALWAYS(dom)
		TRACE_ALWAYS(<< "SVG parsed in " << float(getTicks() - parseStart) / 1000.0f << " sec." << std::endl)
	}
}
