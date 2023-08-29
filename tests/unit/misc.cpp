#include <tst/set.hpp>
#include <tst/check.hpp>

#include <fstream>

#include <papki/fs_file.hpp>

#include <svgdom/dom.hpp>
#include <svgdom/util/style_stack.hpp>
#include <svgdom/util/finder_by_id.hpp>
#include <svgdom/elements/style.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const tst::set set("misc", [](tst::suite& suite){
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
            )qwertyuiop"sv;

            bool thrown = false;
            try{
                auto dom = svgdom::load(str);
                tst::check(false, SL);
            }catch(std::invalid_argument& e){
                thrown = true;
            }
            tst::check(thrown, SL);
        }
    );

    suite.add("issue_test_1", [](){
        auto svg_str = R"qwertyuiop(
            <?xml version="1.0" encoding="UTF-8"?>
            <svg xmlns="http://www.w3.org/2000/svg" id="layer_1" data-name="layer 1" viewBox="0 0 48 48">
            <defs>
                <style id="css">.cls-1,.cls-3{fill:none;}.cls-2,.cls-4{fill:#47506a;}.cls-3{stroke:#3866f0;stroke-width:4px;}.cls-4{fill-rule:evenodd;}</style>
            </defs>
            <g id="main">
                <g id="all tools">
                    <g id="group-13">
                        <g id="drive">
                        <rect id="rect-2" data-name="rect" class="cls-2" x="12" y="32" width="24" height="4" />
                        </g>
                    </g>
                </g>
            </g>
            </svg>
        )qwertyuiop"sv;

        auto dom = svgdom::load(utki::make_span(svg_str));
        tst::check(dom, SL);

        svgdom::style_stack style_stack;

        svgdom::finder_by_id finder(*dom);

        {
            auto elem = finder.find("css");
            tst::check(elem, SL);

            auto style_elem = dynamic_cast<const svgdom::style_element*>(elem);
            tst::check(style_elem, SL);

            style_stack.add_css(style_elem->css);
        }

        auto elem = finder.find("rect-2");
        tst::check(elem, SL);

        auto styleable = dynamic_cast<const svgdom::styleable*>(elem);
        tst::check(styleable, SL);

        svgdom::style_stack::push style_push(style_stack, *styleable);

        auto fill = style_stack.get_style_property(svgdom::style_property::fill);
        tst::check(fill, SL);
        tst::check(std::holds_alternative<uint32_t>(*fill), SL);
        tst::check_eq(std::get<uint32_t>(*fill), 0x6a5047U, SL);
    });
});
}
