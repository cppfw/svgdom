#include "../../src/svgdom/dom.hpp"

#include <utki/debug.hpp>

#include <papki/FSFile.hpp>


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
	
	auto dom = svgdom::load(papki::FSFile(filename));
	
	ASSERT_ALWAYS(dom)
	
	auto str = dom->toString();
//	TRACE_ALWAYS(<< str << std::endl)
	
	papki::FSFile outFile("out.svg");
	papki::File::Guard fileGuard(outFile, papki::File::E_Mode::CREATE);
	outFile.write(utki::wrapBuf<std::uint8_t>(
			reinterpret_cast<const std::uint8_t*>(str.c_str()),
			str.length()
		));
}
