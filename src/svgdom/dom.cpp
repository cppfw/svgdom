/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#include "dom.hpp"
#include "config.hpp"

#include "parser.hxx"

using namespace svgdom;

std::unique_ptr<svg_element> svgdom::load(const papki::file& f){
	svgdom::parser parser;
	
	{
		papki::file::guard file_guard(f);

		std::array<uint8_t, 4096> buf; // 4k

		while(true){
			auto res = f.read(utki::make_span(buf));
			ASSERT(res <= buf.size())
			if(res == 0){
				break;
			}
			parser.feed(utki::make_span(buf.data(), res));
		}
		parser.end();
	}
	
	return parser.get_dom();
}

std::unique_ptr<svg_element> svgdom::load(std::istream& s){
	svgdom::parser parser;
	
	static const size_t chunk_size = 0x1000; // 4kb

	while(!s.eof()){
		std::vector<char> buf;
		buf.reserve(chunk_size);
		for(size_t i = 0; i != chunk_size; ++i){
			char c;
			c = s.get();
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