#pragma once

#include <utki/Exc.hpp>


namespace svgdom{

class Exc : public utki::Exc{
public:
	Exc(const std::string& message = std::string()) :
			utki::Exc(message)
	{}
};

}
