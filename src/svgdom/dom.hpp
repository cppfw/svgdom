/* 
 * File:   dom.hpp
 * Author: ivan
 *
 * Created on October 8, 2015, 3:15 PM
 */

#pragma once

#include <vector>
#include <memory>

#include <papki/File.hpp>

namespace svgdom{

class Element{
public:
	virtual ~Element()noexcept{}
};

class ContainerElement : public Element{
public:
	std::vector<std::unique_ptr<Element>> children;
};

class SvgElement : public ContainerElement{
	
};




std::unique_ptr<SvgElement> load(const papki::File& f);

}//~namespace
