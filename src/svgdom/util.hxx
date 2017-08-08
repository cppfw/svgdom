#pragma once

#include <istream>

#include "config.hpp"
#include "CoordinateUnits.hpp"

namespace svgdom{

void skipWhitespaces(std::istream& s);

void skipWhitespacesAndOrComma(std::istream& s);

void skipTillCharInclusive(std::istream& s, char c);

std::string readTillChar(std::istream& s, char c);

std::string readTillCharOrWhitespace(std::istream& s, char c);

real readInReal(std::istream& s);

std::string trimTail(const std::string& s);

std::string iriToLocalId(const std::string& iri);

CoordinateUnits_e parseCoordinateUnits(const std::string& s);

std::string coordinateUnitsToString(CoordinateUnits_e u);

}
