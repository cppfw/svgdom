#pragma once

#include "config.hpp"

#include <istream>

namespace svgdom{

void skipWhitespaces(std::istream& s);

real readInReal(std::istream& s);

}
