#pragma once

#include <istream>
#include <array>
#include <string_view>

#include <r4/vector.hpp>

#include "config.hpp"
#include "elements/coordinate_units.hpp"

namespace svgdom{

void skip_whitespaces(std::istream& s);

void skip_whitespaces_and_comma(std::istream& s);

size_t skip_whitespaces_and_comma(const std::string_view str);

void skip_till_char_inclusive(std::istream& s, char c);

std::string read_till_char(std::istream& s, char c);

std::string read_till_char_or_whitespace(std::istream& s, char c);

real read_in_real(std::istream& s);

struct parse_real_result{
    real number;
    size_t stop_pos;
};

parse_real_result parse_real(const std::string_view str);

std::string trim_tail(const std::string& s);

std::string iri_to_local_id(const std::string& iri);

coordinate_units parse_coordinate_units(const std::string& s);

std::string coordinate_units_to_string(coordinate_units u);

r4::vector2<real> parse_number_and_optional_number(const std::string& s, r4::vector2<real> defaults);

std::string number_and_optional_number_to_string(std::array<real, 2> non, real optionalNumberDefault);

}
