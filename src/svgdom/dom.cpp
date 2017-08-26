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
	Parser parser;
	
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
		parser.feed(utki::wrapBuf(buf));
	}
	parser.end();
	
	return parser.getDom();
}

std::unique_ptr<SvgElement> svgdom::load(const std::string& s){
	return load(utki::wrapBuf(s.c_str(), s.length()));
}

std::unique_ptr<SvgElement> svgdom::load(const utki::Buf<std::uint8_t> buf){
	return load(utki::wrapBuf(reinterpret_cast<const char*>(&*buf.begin()), buf.size()));
}

std::unique_ptr<SvgElement> svgdom::load(const utki::Buf<char> buf){
	Parser parser;

	parser.feed(buf);
	parser.end();

	return parser.getDom();
}