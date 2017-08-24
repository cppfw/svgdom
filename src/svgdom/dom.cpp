#include <pugixml.hpp>

#include "dom.hpp"
#include "config.hpp"
#include "Exc.hpp"
#include "util.hxx"

#include "Parser.hxx"

using namespace svgdom;



std::unique_ptr<SvgElement> svgdom::load(const papki::File& f){
	Parser parser;
	
	{
		papki::File::Guard fileGuard(f);

		std::array<std::uint8_t, 4096> buf; //4k

		while(true){
			auto res = f.read(utki::wrapBuf(buf));
			ASSERT_ALWAYS(res <= buf.size())
			if(res == 0){
				break;
			}
			parser.feed(utki::wrapBuf(&*buf.begin(), res));
		}
		parser.end();
	}
	
	return parser.getDom();
}

std::unique_ptr<SvgElement> svgdom::load(std::istream& s){
	pugi::xml_document doc;
	
	doc.load(s);
	
	return ::load(doc);
}

std::unique_ptr<SvgElement> svgdom::load(std::string& s){
	pugi::xml_document doc;

	doc.load_string(s.c_str());

	return ::load(doc);
}
