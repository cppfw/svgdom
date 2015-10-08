#include "../../src/svgdom/dom.hpp"

#include <utki/debug.hpp>

#include <papki/FSFile.hpp>


int main(int argc, char** argv){

	auto dom = svgdom::load(papki::FSFile("tiger.svg"));
	
	
	TRACE_ALWAYS(<< "[PASSED]" << std::endl)
}
