#include <tst/set.hpp>
#include <tst/check.hpp>

#include <fstream>

#include <papki/fs_file.hpp>

#include "../../src/svgdom/dom.hpp"

using namespace std::string_literals;

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

    suite.add(
        "read_svg_with_unclosed_svg_tag_should_fail",
        [](){
            auto str = R"qwertyuiop(
                <?xml version="1.0" encoding="utf-8"?>
                <!DOCTYPE svg >
                <svg width="1000px" height="1000px" xmlns="http://www.w3.org/2000/svg">
                    <rect style="fill: hsl(84,39%,99%);" width="100%" height="100%"/>
                    <rect style="fill: hsl(120,42%,71%);" x="93" y="98.5" width="814" height="803"/>
                <!-- /svg -->
            )qwertyuiop"s;
            try{
                auto dom = svgdom::load(str);
                tst::check(false, SL);
            }catch(std::invalid_argument& e){}
        }
    );
});
}
