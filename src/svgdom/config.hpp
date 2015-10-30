/* 
 * File:   config.hpp
 * Author: ivan
 *
 * Created on October 9, 2015, 2:41 PM
 */

#pragma once

#include <utki/config.hpp>

#if M_OS == M_OS_WINDOWS
#	ifdef OVERFLOW
#		undef OVERFLOW
#	endif
#endif

namespace svgdom{

typedef float real;

}
