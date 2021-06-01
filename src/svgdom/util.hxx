#pragma once

#include <istream>
#include <array>
#include <string_view>

#include <r4/vector.hpp>

#include "config.hpp"
#include "elements/coordinate_units.hpp"

namespace svgdom{

class string_parser{
    std::string_view view;
public:
    string_parser(std::string_view view) :
            view(view)
    {}

    void skip_whitespaces();
    void skip_whitespaces_and_comma();

    std::string_view read_word();

    // skips leading whitespaces
    real read_real();

    bool empty()const noexcept{
        return this->view.empty();
    }

    std::string_view get_view()const noexcept{
        return this->view;
    }
};

void skip_whitespaces(std::istream& s);

std::string_view skip_whitespaces(std::string_view s);

void skip_whitespaces_and_comma(std::istream& s);

std::string_view skip_whitespaces_and_comma(std::string_view str);

void skip_till_char_inclusive(std::istream& s, char c);

std::string read_till_char(std::istream& s, char c);

std::string read_till_char_or_whitespace(std::istream& s, char c);

real read_in_real(std::istream& s);

struct read_word_result{
	std::string_view word;
	std::string_view view;
};

struct parse_real_result{
    real number;
    std::string_view view; // view after parsing
	bool error = false;
};

parse_real_result parse_real(std::string_view str);

std::string trim_tail(const std::string& s);

std::string iri_to_local_id(const std::string& iri);

coordinate_units parse_coordinate_units(const std::string& s);

std::string coordinate_units_to_string(coordinate_units u);

r4::vector2<real> parse_number_and_optional_number(std::string_view s, r4::vector2<real> defaults);

std::string number_and_optional_number_to_string(std::array<real, 2> non, real optionalNumberDefault);

}
