#pragma once

#include <ostream>

#include <utki/Unique.hpp>

//TODO: remove
#include "Styleable.hpp"

namespace svgdom{

//TODO: remove
struct Container;

class Visitor;

/**
 * @brief Base class for all SVG document elements.
 */
struct Element : public utki::Unique{
	//TODO: remove
	Container* parent = nullptr;
	
	std::string id;
	
	virtual ~Element()noexcept{}
	
	//TODO: move all attribsToStream to cpp
	void attribsToStream(std::ostream& s)const;
	
	//TODO: implement via Visitor
	virtual void toStream(std::ostream& s, unsigned indent = 0)const = 0;
	
	//TODO: implement via Visitor
	std::string toString()const;
	
	/**
	 * @brief Accept method for Visitor pattern.
	 * @param visitor - visitor to accept.
	 */
	virtual void accept(Visitor& visitor)const = 0;
	
	/**
	 * @brief Get style property of the element.
	 * Properties are CSS, so it will get the property from parent elements in case this element
	 * does not define the property.
     * @param property - property to get.
     * @param explicitInherit - the flag is used to indicate that an explicit inheritance is indicated for non-inherited properties.
     * @return pointer to the property value.
	 * @return nullptr in case property was not found.
     */
	//TODO: remove
	const StylePropertyValue* getStyleProperty(StyleProperty_e property, bool explicitInherit = false)const;
	
	/**
	 * @brief Find element by its id.
	 * Searches the elements tree to find an element with indicated id.
     * @param elementId - element id to search for.
     * @return pointer to the found element.
	 * @return nullptr in case the element with given id was not found.
     */
	//TODO: implement via Visitor
	virtual Element* findById(const std::string& elementId);
};

}
