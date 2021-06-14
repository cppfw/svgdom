#pragma once

#include <array>
#include <string_view>
#include <charconv>

#include <r4/vector.hpp>

#include <utki/string.hpp>

#include "config.hpp"
#include "elements/coordinate_units.hpp"

namespace svgdom{

std::string_view trim_tail(std::string_view s);

std::string iri_to_local_id(std::string_view iri);

coordinate_units parse_coordinate_units(std::string_view s);

std::string coordinate_units_to_string(coordinate_units u);

r4::vector2<real> parse_number_and_optional_number(std::string_view s, r4::vector2<real> defaults);

std::string number_and_optional_number_to_string(std::array<real, 2> non, real optional_number_default);

}
