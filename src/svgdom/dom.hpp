#pragma once

#include <utki/config.hpp>

#include <papki/file.hpp>

#include "elements/structurals.hpp"
#include "stream_writer.hpp"

namespace svgdom{

/**
 * @brief Load SVG document.
 * Load SVG document from XML file.
 * @param f - file interface to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<svg_element> load(const papki::file& f);

/**
 * @brief Load SVG document.
 * Load SVG document from XML stream.
 * @param s - input stream to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<svg_element> load(std::istream& s);

/**
 * @brief Load SVG document.
 * Load SVG document from std::string.
 * @param s - input string to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<svg_element> load(const std::string& s);

/**
 * @brief Load SVG document from memory buffer.
 * @param buf - input buffer to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<svg_element> load(utki::span<const char> buf);

/**
 * @brief Load SVG document from memory buffer.
 * @param buf - input buffer to load SVG from.
 * @return unique pointer to the root of SVG document tree.
 */
std::unique_ptr<svg_element> load(utki::span<const uint8_t> buf);

}
