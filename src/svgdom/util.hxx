#pragma once

#include "config.hpp"

#include <istream>

namespace svgdom{

void skipWhitespaces(std::istream& s);

void skipWhitespacesAndOrComma(std::istream& s);

void skipTillCharInclusive(std::istream& s, char c);

std::string readTillChar(std::istream& s, char c);

std::string readTillCharOrWhitespace(std::istream& s, char c);

real readInReal(std::istream& s);

std::string trimTail(const std::string& s);

}
