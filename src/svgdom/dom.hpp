/* 
 * File:   dom.hpp
 * Author: ivan
 *
 * Created on October 8, 2015, 3:15 PM
 */

#pragma once

#include <vector>
#include <memory>

#include <utki/Unique.hpp>
#include <papki/File.hpp>


namespace svgdom{

enum class EUnit{
	UNKNOWN,
	NUMBER,
	PERCENT,
	EM,
	EX,
	PX,
	CM,
	IN,
	PT,
	PC
};

struct Length{
	float value = 0;
	EUnit unit = EUnit::NUMBER;
};

struct Element : public utki::Unique{
	std::string id;
	
	virtual ~Element()noexcept{}
};

struct Container{
	std::vector<std::unique_ptr<Element>> children;
};

struct Rectangle{
	Length x, y, width, height;
};

struct SvgElement : public Element, public Container, public Rectangle{
	
};




std::unique_ptr<SvgElement> load(const papki::File& f);

}//~namespace
