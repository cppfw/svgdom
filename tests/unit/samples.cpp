#include <utki/config.hpp>

#if CFG_COMPILER != CFG_COMPILER_MSVC

#include <tst/set.hpp>
#include <tst/check.hpp>

#include <fsif/native_file.hpp>
#include <fsif/vector_file.hpp>

#include <utki/linq.hpp>

#include <regex>
#include <clocale>

#include "../../src/svgdom/dom.hpp"

namespace{
const std::string data_dir = "samples_data/";
}

namespace{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const tst::set set("samples", [](tst::suite& suite){
    // make sure the locale does not affect parsing (decimal delimiter can be "." or "," in different locales)
	// so, set DE locale which has "," to make sure it does not affect the parsing
	if(!std::setlocale(LC_ALL, "de_DE.UTF-8")){
		utki::log([](auto& o){o << "WARNING: failed to set locale de_DE.UTF-8, perhaps the locale is not installed. Testing that locale does not affect parsing will not be done." << std::endl;});
	}

    std::vector<std::string> files = utki::linq(fsif::native_file(data_dir).list_dir())
            .where(
                    [&](const auto& f){
                        static const std::regex suffix_regex("^.*\\.svg$");
                        return std::regex_match(f, suffix_regex);
                    }
                )
            .get();

    suite.add<std::string>(
        "sample",
        std::move(files),
        [](auto& p){
            auto in_file_name = data_dir + p;

            auto dom = svgdom::load(fsif::native_file(in_file_name));
            tst::check(dom, SL);
            
            auto str = dom->to_string();
            
            fsif::vector_file out_file;
            {
                fsif::file::guard file_guard(out_file, fsif::mode::create);
                out_file.write(utki::make_span(str));
            }
            
            auto out_data = out_file.reset_data();

            fsif::native_file cmp_file(in_file_name + ".cmp");

            decltype(out_data) cmp_data;

            try{
                cmp_data = cmp_file.load();
            }catch(std::system_error&){
                std::cout << "Failed to load '" << cmp_file.path() << "' file" << std::endl;
            }

            if(out_data != cmp_data){
                fsif::native_file failed_file(in_file_name + ".out");

                {
                    fsif::file::guard file_guard(failed_file, fsif::mode::create);
                    failed_file.write(out_data);
                }

                tst::check(false, SL) << "parsed file is not as expected: " << in_file_name;
            }
        }
    );
});
}

#endif // ~ non-MSVC compiler or MSVC tools is v142+
