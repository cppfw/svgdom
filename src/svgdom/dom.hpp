#pragma once

#include <memory>
#include <ostream>

#include <utki/Unique.hpp>
#include <utki/Void.hpp>
#include <utki/config.hpp>

#include <papki/File.hpp>

#include "config.hpp"
#include "Length.hpp"
#include "elements/Styleable.hpp"
#include "elements/Element.hpp"
#include "elements/Container.hpp"
#include "elements/Referencing.hpp"
#include "elements/Transformable.hpp"
#include "elements/ViewBoxed.hpp"
#include "elements/Rectangle.hpp"
#include "elements/Structurals.hpp"
#include "elements/Shapes.hpp"
#include "elements/Gradients.hpp"


namespace svgdom{

/**
 * @brief Load SVG document.
 * Load SVG document from XML file.
 * @param f - file interface to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(const papki::File& f);

/**
 * @brief Load SVG document.
 * Load SVG document from XML stream.
 * @param s - input stream to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(std::istream& s);

/**
 * @brief Load SVG document.
 * Load SVG document from std::string.
 * @param s - input string to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(std::string& s);

}
