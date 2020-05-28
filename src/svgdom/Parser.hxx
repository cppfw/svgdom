#pragma once

#include <map>
#include <vector>
#include <memory>

#include <mikroxml/mikroxml.hpp>

#include "elements/element.hpp"
#include "elements/referencing.hpp"
#include "elements/rectangle.hpp"
#include "elements/view_boxed.hpp"
#include "elements/transformable.hpp"
#include "elements/gradients.hpp"
#include "elements/shapes.hpp"
#include "elements/structurals.hpp"
#include "elements/filter.hpp"
#include "elements/image_element.hpp"
#include "elements/text_element.hpp"
#include "elements/style.hpp"

namespace svgdom{

class Parser : public mikroxml::parser{
	enum class XmlNamespace_e{
		ENUM_FIRST,
		UNKNOWN = ENUM_FIRST,
		SVG,
		XLINK,
		
		ENUM_SIZE
	};
	
	std::vector<
			std::map<std::string, XmlNamespace_e>
		> namespacesStack;
	std::vector<
			std::map<XmlNamespace_e, std::string>
		> flippedNamespacesStack;
	
	std::vector<XmlNamespace_e> defaultNamespaceStack;
	
	
	XmlNamespace_e findNamespace(const std::string& ns);
	const std::string* findFlippedNamespace(XmlNamespace_e ns);
	
	struct NamespaceNamePair{
		XmlNamespace_e ns;
		std::string name;
	};
	
	NamespaceNamePair getNamespace(const std::string& xmlName);
	
	const std::string* findAttribute(const std::string& name);
	
	const std::string* findAttributeOfNamespace(XmlNamespace_e ns, const std::string& name);

	void pushNamespaces();
	void popNamespaces();
	
	std::string cur_element;
	std::map<std::string, std::string> attributes;
	
	std::unique_ptr<svg_element> svg; // root svg element
	std::vector<container*> element_stack;
	
	void addElement(std::unique_ptr<element> e);
	void addElement(std::unique_ptr<element> e, container* c);
	
	void on_element_start(utki::span<const char> name) override;
	void on_element_end(utki::span<const char> name) override;
	void on_attribute_parsed(utki::span<const char> name, utki::span<const char> value) override;
	void on_attributes_end(bool is_empty_element) override;
	void on_content_parsed(utki::span<const char> str) override;

	void fillElement(Element& e);
	void fillReferencing(Referencing& e);
	void fillRectangle(
			rectangle& r,
			const rectangle& defaultValues = rectangle(
					length::make(0, length_unit::percent),
					length::make(0, length_unit::percent),
					length::make(100, length_unit::percent),
					length::make(100, length_unit::percent)
				)
		);
	void fillViewBoxed(ViewBoxed& v);
	void fillAspectRatioed(AspectRatioed& e);
	void fillTransformable(Transformable& t);
	void fillStyleable(Styleable& s);
	void fillGradient(Gradient& g);
	void fillShape(Shape& s);
	void fillFilterPrimitive(FilterPrimitive& p);
	void fillInputable(Inputable& p);
	void fillSecondInputable(SecondInputable& p);
	void fillTextPositioning(TextPositioning& p);
	void fill_style(style_element& e);
	
	void parseGradientStopElement();
	void parseSvgElement();
	void parseSymbolElement();
	void parseGElement();
	void parseDefsElement();
	void parseUseElement();
	void parsePathElement();
	void parseRectElement();
	void parseCircleElement();
	void parseLineElement();
	void parsePolylineElement();
	void parsePolygonElement();
	void parseEllipseElement();
	void parseLinearGradientElement();
	void parseRadialGradientElement();
	void parseFilterElement();
	void parseFeGaussianBlurElement();
	void parseFeColorMatrixElement();
	void parseFeBlendElement();
	void parseFeCompositeElement();
	void parseImageElement();
	void parseMaskElement();
	void parseTextElement();
	void parse_style_element();
	
	void parse_element();
public:
	std::unique_ptr<SvgElement> get_dom();
};

}
