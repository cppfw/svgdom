#pragma once

#include <utki/config.hpp>

#include <papki/File.hpp>

#include "elements/Structurals.hpp"
#include "StreamWriter.hpp"


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
std::unique_ptr<SvgElement> load(const std::string& s);

/**
 * @brief Load SVG document from memory buffer.
 * @param buf - input buffer to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(const utki::Buf<char> buf);

/**
 * @brief Load SVG document from memory buffer.
 * @param buf - input buffer to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<SvgElement> load(const utki::Buf<std::uint8_t> buf);

}
