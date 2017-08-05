#pragma once

#include <string>
#include "Element.hpp"

namespace svgdom{

/**
 * @brief Element representing a custom string to insert in XML.
 */
struct CustomStringElement : public Element
{
  std::string customString;
  
	CustomStringElement();
	
	CustomStringElement(std::string s);
	
  std::string getCustomString() const;
  
  void setCustomString(std::string s);
  
  void accept(Visitor& visitor) const override;
};


}