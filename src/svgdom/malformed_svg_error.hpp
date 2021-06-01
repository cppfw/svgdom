#pragma once

#include <stdexcept>

namespace svgdom{

class malformed_svg_error : public std::invalid_argument{
public:
	malformed_svg_error(const std::string& message) :
			std::invalid_argument(message)
	{}
};

}
