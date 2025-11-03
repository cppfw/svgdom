#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/time.hpp>
#include <fsif/native_file.hpp>

#include "../../src/svgdom/dom.hpp"

namespace{
const tst::set set("performance", [](auto& suite){
	suite.add("basic_test", [](){
		auto load_start = utki::get_ticks_ms();
	
		auto buf = fsif::native_file("samples_data/back.svg").load();
		
		utki::log([&](auto&o){o << "SVG loaded in " << float(utki::get_ticks_ms() - load_start) / 1000.0f << " sec." << std::endl;});
		
		for(unsigned i = 0; i != 5; ++i){
			auto parse_start = utki::get_ticks_ms();
			auto dom = svgdom::load(utki::make_span(buf));
			tst::check(dom != nullptr, SL);
			utki::log([&](auto&o){o << "SVG parsed in " << float(utki::get_ticks_ms() - parse_start) / 1000.0f << " sec." << std::endl;});
		}
	});
});
}
