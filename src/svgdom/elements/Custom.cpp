#include "Custom.hpp"

#include "../util.hxx"
#include "../Visitor.hpp"

#include <sstream>
#include <cctype>

#include <utki/debug.hpp>


using namespace svgdom;

std::string CustomStringElement::getCustomString() const {
  return customString;
}

void CustomStringElement::setCustomString(std::string s) {
  customString = s;
}

void CustomStringElement::accept(Visitor& visitor) const {
  visitor.visit(*this);
}

void CustomStringElement::attribsToStream(std::ostream& s) const {
}