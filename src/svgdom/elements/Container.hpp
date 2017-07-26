#pragma once

#include <vector>

#include "Element.hpp"

namespace svgdom{

/**
 * @brief An element which can have child elements.
 */
//TODO: do not derive from Element
struct Container : public Element{
	std::vector<std::unique_ptr<Element>> children;
	
	//TODO: move to cpp
	void childrenToStream(std::ostream& s, unsigned indent)const;
	
	/**
	 * @brief Relay accept to children.
	 * @param visitor - visitor to accept.
	 */
	void relayAccept(Visitor& visitor)const;
	
	Element* findById(const std::string& elementId) override;
};

}
