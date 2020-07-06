#include "../../src/svgdom/dom.hpp"

#include <utki/debug.hpp>

#include <papki/fs_file.hpp>

int main(int argc, char** argv){
	std::string filename;
	switch(argc){
		case 0:
		case 1:
			std::cout << "Warning: expected 1 argument: <svg-file>" << std::endl;
			std::cout << "\tGot 0 arguments, assuming <svg-file>=tiger.svg" << std::endl;
			filename = "tiger.svg";
			break;
		default:
		case 2:
			filename = argv[1];
			break;
	}
	
	auto dom = svgdom::load(papki::fs_file(filename));
	
//	TRACE_ALWAYS(<< "file read" << std::endl)
	
	ASSERT_ALWAYS(dom)
	
	auto str = dom->to_string();
//	TRACE_ALWAYS(<< str << std::endl)
	
	papki::fs_file outFile("out.svg");
	papki::file::guard fileGuard(outFile, papki::file::mode::create);
	outFile.write(utki::make_span(str));
}
