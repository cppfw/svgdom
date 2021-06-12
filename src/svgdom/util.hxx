#pragma once

#include <array>
#include <string_view>
#include <charconv>

#include <r4/vector.hpp>

#include <utki/string.hpp>

#include "config.hpp"
#include "elements/coordinate_units.hpp"

namespace svgdom{

class string_parser{
    std::string_view view;

    void throw_if_empty();

public:
    static bool is_space(char c);

    string_parser(std::string_view view) :
            view(view)
    {}

    void skip_whitespaces();
    void skip_whitespaces_and_comma();
    void skip_inclusive_until(char c);

    std::string_view read_word();
    std::string_view read_word_until(char c);

    // skips leading whitespaces
    template <class real_type>
    real_type read_real(){
        this->skip_whitespaces();

        std::conditional_t<
                std::is_same<real_type, float>::value || std::is_same<real_type, double>::value,
                real_type,
                double // TODO: use long double when std::from_chars for floats is widely supported by C++17 compilers
            > ret;

        // TODO: use std::from_chars for floats when it is widely supported by C++17 compilers
        auto res = utki::from_chars(this->view.data(), this->view.data() + this->view.size(), ret);

        if(res.ec == std::errc::invalid_argument){
            throw std::invalid_argument("string_parser::read_real(): could not parse real number");
        }

        ASSERT(this->view.data() != res.ptr)

        this->view = this->view.substr(res.ptr - this->view.data());

        return real_type(ret);
    }

    // skips leading whitespaces
    template <class integer_type>
    integer_type read_integer(){
        this->skip_whitespaces();

        integer_type ret = 0;

        auto res = std::from_chars(this->view.data(), this->view.data() + this->view.size(), ret);

        if(res.ec == std::errc::invalid_argument){
            throw std::invalid_argument("string_parser::read_integer(): could not parse integer number");
        }

        ASSERT(this->view.data() != res.ptr)

        this->view = this->view.substr(res.ptr - this->view.data());

        return ret;
    }

    char read_char();

    char peek_char();

    std::string_view read_chars(size_t n);
    std::string_view read_chars_until(char c);

    bool empty()const noexcept{
        return this->view.empty();
    }
};

std::string_view trim_tail(std::string_view s);

std::string iri_to_local_id(std::string_view iri);

coordinate_units parse_coordinate_units(std::string_view s);

std::string coordinate_units_to_string(coordinate_units u);

r4::vector2<real> parse_number_and_optional_number(std::string_view s, r4::vector2<real> defaults);

std::string number_and_optional_number_to_string(std::array<real, 2> non, real optional_number_default);

}
