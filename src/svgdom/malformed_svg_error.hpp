#pragma once

#include <stdexcept>

namespace svgdom{

class malformed_svg_error : public std::logic_error{
public:
	malformed_svg_error(const std::string& message) :
			std::logic_error(message)
	{}
};

}
