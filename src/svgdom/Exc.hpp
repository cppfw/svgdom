/* 
 * File:   Exc.hpp
 * Author: ivan
 *
 * Created on October 14, 2015, 11:34 PM
 */

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
