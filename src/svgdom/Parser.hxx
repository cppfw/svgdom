#pragma once

#include <map>
#include <vector>
#include <memory>

#include <pugixml.hpp>

#include "elements/Element.hpp"
#include "elements/Referencing.hpp"
#include "elements/Rectangle.hpp"
#include "elements/ViewBoxed.hpp"
#include "elements/Transformable.hpp"
#include "elements/Gradients.hpp"
#include "elements/Shapes.hpp"
#include "elements/Structurals.hpp"

namespace svgdom{

class Parser{
	enum class XmlNamespace_e{
		UNKNOWN,
		SVG,
		XLINK
	};
	
	std::vector<
			std::map<std::string, XmlNamespace_e>
		> namespaces;
	
	std::vector<XmlNamespace_e> defaultNamespace;
	
	
	XmlNamespace_e findNamespace(const std::string& ns);
	
	struct NamespaceNamePair{
		XmlNamespace_e ns;
		std::string name;
	};
	
	NamespaceNamePair getNamespace(const std::string& xmlAttributeName);

	void fillElement(Element& e, const pugi::xml_node& n);
	void fillReferencing(Referencing& e, const pugi::xml_node& n);
	void fillRectangle(Rectangle& r, const pugi::xml_node& n);
	void fillViewBoxed(ViewBoxed& v, const pugi::xml_node& n);
	void fillContainer(Container& c, const pugi::xml_node& n);
	void fillTransformable(Transformable& t, const pugi::xml_node& n);
	void fillStyleable(Styleable& s, const pugi::xml_node& n);
	void fillGradient(Gradient& g, const pugi::xml_node& n);
	void fillShape(Shape& s, const pugi::xml_node& n);
	
	std::unique_ptr<Gradient::StopElement> parseGradientStopElement(const pugi::xml_node& n);
	std::unique_ptr<SvgElement> parseSvgElement(const pugi::xml_node& n);
	std::unique_ptr<SymbolElement> parseSymbolElement(const pugi::xml_node& n);
	std::unique_ptr<GElement> parseGElement(const pugi::xml_node& n);
	std::unique_ptr<DefsElement> parseDefsElement(const pugi::xml_node& n);
	std::unique_ptr<UseElement> parseUseElement(const pugi::xml_node& n);
	std::unique_ptr<PathElement> parsePathElement(const pugi::xml_node& n);
	std::unique_ptr<RectElement> parseRectElement(const pugi::xml_node& n);
	std::unique_ptr<CircleElement> parseCircleElement(const pugi::xml_node& n);
	std::unique_ptr<LineElement> parseLineElement(const pugi::xml_node& n);
	std::unique_ptr<PolylineElement> parsePolylineElement(const pugi::xml_node& n);
	std::unique_ptr<PolygonElement> parsePolygonElement(const pugi::xml_node& n);
	std::unique_ptr<EllipseElement> parseEllipseElement(const pugi::xml_node& n);
	std::unique_ptr<LinearGradientElement> parseLinearGradientElement(const pugi::xml_node& n);
	std::unique_ptr<RadialGradientElement> parseRadialGradientElement(const pugi::xml_node& n);
	
public:
	std::unique_ptr<svgdom::Element> parseNode(const pugi::xml_node& n);
};

}
