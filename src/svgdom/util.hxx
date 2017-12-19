#pragma once

#include <istream>
#include <array>

#include "config.hpp"
#include "elements/CoordinateUnits.hpp"

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

std::array<real, 2> parseNumberOptionalNumber(const std::string& s, std::array<real, 2> defaults);

std::string numberOptionalNumberToString(std::array<real, 2> non, real optionalNumberDefault);

}
