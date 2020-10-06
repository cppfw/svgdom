#include "dom.hpp"
#include "config.hpp"
#include "util.hxx"

#include "parser.hxx"

using namespace svgdom;

std::unique_ptr<svg_element> svgdom::load(const papki::file& f){
	svgdom::parser parser;
	
	{
		papki::file::guard file_guard(f);

		std::array<uint8_t, 4096> buf; // 4k

		while(true){
			auto res = f.read(utki::make_span(buf));
			ASSERT_ALWAYS(res <= buf.size())
			if(res == 0){
				break;
			}
			parser.feed(utki::make_span(&*buf.begin(), res));
		}
		parser.end();
	}
	
	return parser.get_dom();
}

std::unique_ptr<svg_element> svgdom::load(std::istream& s){
	svgdom::parser parser;
	
	while(!s.eof()){
		std::vector<char> buf;
		for(unsigned i = 0; i != 4096; ++i){
			char c;
			s >> c;
			if(s.eof()){
				break;
			}
			buf.push_back(c);
		}
		parser.feed(utki::make_span(buf));
	}
	parser.end();
	
	return parser.get_dom();
}

std::unique_ptr<svg_element> svgdom::load(const std::string& s){
	return load(utki::make_span(s));
}

std::unique_ptr<svg_element> svgdom::load(utki::span<const uint8_t> buf){
	return load(utki::make_span(reinterpret_cast<const char*>(buf.data()), buf.size()));
}

std::unique_ptr<svg_element> svgdom::load(utki::span<const char> buf){
	svgdom::parser parser;

	parser.feed(buf);
	parser.end();

	return parser.get_dom();
}