#include <tst/set.hpp>
#include <tst/check.hpp>

#include <fstream>

#include <papki/fs_file.hpp>

#include "../../src/svgdom/dom.hpp"

namespace{
tst::set set("misc", [](tst::suite& suite){
    suite.add(
        "read_from_istream",
        [](){
            std::ifstream ist;
            ist.open("samples_data/tiger.svg");
            auto dom = svgdom::load(ist);
            tst::check(dom, SL);
        }
    );

    suite.add(
        "read_from_fs_file",
        [](){
            auto dom = svgdom::load(papki::fs_file("samples_data/tiger.svg"));
            tst::check(dom, SL);
        }
    );
});
}
