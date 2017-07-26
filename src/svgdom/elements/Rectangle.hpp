#pragma once

#include "../Length.hpp"

namespace svgdom{

/**
 * @brief A rectangular element.
 */
struct Rectangle{
	Length x = Length::make(0, Length::Unit_e::PERCENT);
	Length y = Length::make(0, Length::Unit_e::PERCENT);
	Length width = Length::make(100, Length::Unit_e::PERCENT);
	Length height = Length::make(100, Length::Unit_e::PERCENT);
	
	void attribsToStream(std::ostream& s)const;
};

}
