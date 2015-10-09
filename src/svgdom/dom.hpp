/* 
 * File:   dom.hpp
 * Author: ivan
 *
 * Created on October 8, 2015, 3:15 PM
 */

#pragma once

#include <vector>
#include <memory>
#include <ostream>

#include <utki/Unique.hpp>
#include <papki/File.hpp>


#include "config.hpp"


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
	real value;
	EUnit unit;
	
	Length(real value = 0, EUnit unit = EUnit::NUMBER) :
			value(value),
			unit(unit)
	{}
	
	static Length parse(const std::string& str);
};

struct Element : public utki::Unique{
	std::string id;
	
	virtual ~Element()noexcept{}
	
	void attribsToStream(std::ostream& s)const;
	
	virtual void toStream(std::ostream& s, unsigned indent = 0)const = 0;
	
	std::string toString()const;
};

struct Container{
	std::vector<std::unique_ptr<Element>> children;
	
	void childrenToStream(std::ostream& s, unsigned indent)const;
};

struct Rectangle{
	Length x, y;
	Length width = Length(100, EUnit::PERCENT);
	Length height = Length(100, EUnit::PERCENT);
	
	void attribsToStream(std::ostream& s)const;
};

struct SvgElement : public Element, public Container, public Rectangle{
	void toStream(std::ostream& s, unsigned indent = 0)const override;
};




std::unique_ptr<SvgElement> load(const papki::File& f);

}//~namespace


std::ostream& operator<<(std::ostream& s, const svgdom::Length& l);
