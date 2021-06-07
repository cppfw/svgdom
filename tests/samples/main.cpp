#include "../../src/svgdom/dom.hpp"

#include <clocale>

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

	// make sure the locale does not affect parsing (decimal delimiter can be "." or "," in different locales)
	// so, set DE locale which has "," to make sure it does not affect the parsing
	if(!std::setlocale(LC_ALL, "de_DE.UTF-8")){
		utki::log([](auto& o){o << "WARNING: failed to set locale de_DE.UTF-8, perhaps the locale is not installed. Testing that locale does not affect parsing will not be done.";});
	}
	
	auto dom = svgdom::load(papki::fs_file(filename));
	
//	TRACE_ALWAYS(<< "file read" << std::endl)
	
	ASSERT_ALWAYS(dom)
	
	auto str = dom->to_string();
//	TRACE_ALWAYS(<< str << std::endl)
	
	papki::fs_file out_file("out.svg");
	papki::file::guard file_guard(out_file, papki::file::mode::create);
	out_file.write(utki::make_span(str));
}
