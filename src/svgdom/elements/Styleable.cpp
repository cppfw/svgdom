#include "Styleable.hpp"

#include <set>

#include <utki/debug.hpp>
#include <utki/util.hpp>

#include "../util.hxx"

using namespace svgdom;


void Styleable::attribsToStream(std::ostream& s) const{
	if(this->styles.size() == 0){
		return;
	}
	
	s << " style=\"";
	
	bool isFirst = true;
	
	for(auto& st : this->styles){
		if(isFirst){
			isFirst = false;
		}else{
			s << "; ";
		}
		
		ASSERT(st.first != StyleProperty_e::UNKNOWN)
		
		s << propertyToString(st.first) << ":";
		
		switch(st.first){
			default:
				ASSERT(false)
				break;
			case StyleProperty_e::STOP_OPACITY:
			case StyleProperty_e::OPACITY:
			case StyleProperty_e::STROKE_OPACITY:
			case StyleProperty_e::FILL_OPACITY:
				s << st.second.opacity;
				break;
			case StyleProperty_e::STOP_COLOR:
			case StyleProperty_e::FILL:
			case StyleProperty_e::STROKE:
				s << st.second.paintToString();
				break;
			case StyleProperty_e::STROKE_WIDTH:
				s << st.second.length;
				break;
			case StyleProperty_e::STROKE_LINECAP:
				switch(st.second.strokeLineCap){
					default:
						ASSERT(false)
						break;
					case StrokeLineCap_e::BUTT:
						s << "butt";
						break;
					case StrokeLineCap_e::ROUND:
						s << "round";
						break;
					case StrokeLineCap_e::SQUARE:
						s << "square";
						break;
				}
				break;
			case StyleProperty_e::STROKE_LINEJOIN:
				switch(st.second.strokeLineJoin){
					default:
						ASSERT(false)
						break;
					case StrokeLineJoin_e::MITER:
						s << "miter";
						break;
					case StrokeLineJoin_e::ROUND:
						s << "round";
						break;
					case StrokeLineJoin_e::BEVEL:
						s << "bevel";
						break;
				}
				break;
			case StyleProperty_e::FILL_RULE:
				switch(st.second.fillRule){
					default:
						ASSERT(false)
						break;
					case FillRule_e::EVENODD:
						s << "evenodd";
						break;
					case FillRule_e::NONZERO:
						s << "nonzero";
						break;
				}
				break;
		}
	}
	
	s << "\"";
}

//input parameter 'str' should have no leading or trailing white spaces
StylePropertyValue Styleable::parseStylePropertyValue(StyleProperty_e type, const std::string& str){
	StylePropertyValue v;

	if (str == "inherit") {
		v.type = StylePropertyValue::Type_e::INHERIT;
		return v;
	}

	switch(type){
		default:
			ASSERT(false)
			break;
		case StyleProperty_e::STOP_OPACITY:
		case StyleProperty_e::OPACITY:
		case StyleProperty_e::STROKE_OPACITY:
		case StyleProperty_e::FILL_OPACITY:
			{
				std::istringstream iss(str);
				v.opacity = readInReal(iss);
				utki::clampRange(v.opacity, real(0), real(1));
			}
			break;
		case StyleProperty_e::STOP_COLOR:
		case StyleProperty_e::FILL:
		case StyleProperty_e::STROKE:
			v = StylePropertyValue::parsePaint(str);
//				TRACE(<< "paint read = " << std::hex << v.integer << std::endl)
			break;
		case StyleProperty_e::STROKE_WIDTH:
			v.length = Length::parse(str);
//				TRACE(<< "stroke-width read = " << v.length << std::endl)
			break;
		case StyleProperty_e::STROKE_LINECAP:
			if(str == "butt"){
				v.strokeLineCap = StrokeLineCap_e::BUTT;
			}else if(str == "round"){
				v.strokeLineCap = StrokeLineCap_e::ROUND;
			}else if(str == "square"){
				v.strokeLineCap = StrokeLineCap_e::SQUARE;
			}else{
				TRACE(<< "unknown strokeLineCap value:" << str << std::endl)
			}
			break;
		case StyleProperty_e::STROKE_LINEJOIN:
			if(str == "miter"){
				v.strokeLineJoin = StrokeLineJoin_e::MITER;
			}else if(str == "round"){
				v.strokeLineJoin = StrokeLineJoin_e::ROUND;
			}else if(str == "bevel"){
				v.strokeLineJoin = StrokeLineJoin_e::BEVEL;
			}else{
				TRACE(<< "unknown strokeLineJoin value:" << str << std::endl)
			}
			break;
		case StyleProperty_e::FILL_RULE:
			if(str == "nonzero"){
				v.fillRule = FillRule_e::NONZERO;
			}else if(str == "evenodd"){
				v.fillRule = FillRule_e::EVENODD;
			}else{
				TRACE(<< "unknown fill-rule value:" << str << std::endl)
			}
			break;
	}
	return v;
}


namespace{
StylePropertyValue parseStylePropertyValue(StyleProperty_e type, std::istream& s){
	skipWhitespaces(s);
	std::string str = readTillChar(s, ';');
	str = trimTail(str);
	return Styleable::parseStylePropertyValue(type, str);
}
}

decltype(Styleable::styles) Styleable::parse(const std::string& str){
	std::istringstream s(str);
	
	s >> std::skipws;
	
	decltype(Styleable::styles) ret;
	
	while(!s.eof()){
		skipWhitespaces(s);
		std::string property = readTillCharOrWhitespace(s, ':');
		
		StyleProperty_e type = Styleable::stringToProperty(property);
		
		if(type == StyleProperty_e::UNKNOWN){
			//unknown style property, skip it
			TRACE(<< "Unknown style property: " << property << std::endl)
			skipTillCharInclusive(s, ';');
			continue;
		}
		
		if(s.get() != ':'){
			return ret;//expected colon
		}
		
		StylePropertyValue v = ::parseStylePropertyValue(type, s);
		
		skipWhitespaces(s);
		
		if(!s.eof() && s.get() != ';'){
			return ret;//expected semicolon
		}
		
		ret[type] = std::move(v);
	}
	
	return ret;
}

namespace{
const std::set<StyleProperty_e> nonInheritedStyleProperties = {
	StyleProperty_e::ALIGNMENT_BASELINE,
	StyleProperty_e::BASELINE_SHIFT,
	StyleProperty_e::CLIP,
	StyleProperty_e::CLIP_PATH,
	StyleProperty_e::DISPLAY,
	StyleProperty_e::DOMINANT_BASELINE,
	StyleProperty_e::ENABLE_BACKGROUND,
	StyleProperty_e::FILTER,
	StyleProperty_e::FLOOD_COLOR,
	StyleProperty_e::FLOOD_OPACITY,
	StyleProperty_e::LIGHTING_COLOR,
	StyleProperty_e::MASK,
	StyleProperty_e::OPACITY,
	StyleProperty_e::OVERFLOW,
	StyleProperty_e::STOP_COLOR,
	StyleProperty_e::STOP_OPACITY,
	StyleProperty_e::TEXT_DECORATION,
	StyleProperty_e::UNICODE_BIDI
};
}

bool Styleable::isStylePropertyInherited(StyleProperty_e p) {
	return nonInheritedStyleProperties.find(p) == nonInheritedStyleProperties.end();
}

StyleProperty_e Styleable::stringToProperty(std::string str){
	if(str == "fill"){
		return StyleProperty_e::FILL;
	}else if(str == "fill-opacity"){
		return StyleProperty_e::FILL_OPACITY;
	}else if(str == "stroke"){
		return StyleProperty_e::STROKE;
	}else if(str == "stroke-width"){
		return StyleProperty_e::STROKE_WIDTH;
	}else if(str == "stroke-linecap"){
		return StyleProperty_e::STROKE_LINECAP;
	}else if(str == "stroke-linejoin"){
		return StyleProperty_e::STROKE_LINEJOIN;
	}else if(str == "stroke-opacity"){
		return StyleProperty_e::STROKE_OPACITY;
	}else if(str == "opacity"){
		return StyleProperty_e::OPACITY;
	}else if(str == "stop-opacity"){
		return StyleProperty_e::STOP_OPACITY;
	}else if(str == "stop-color"){
		return StyleProperty_e::STOP_COLOR;
	}else if(str == "fill-rule"){
		return StyleProperty_e::FILL_RULE;
	}
	
	return StyleProperty_e::UNKNOWN;
}

std::string Styleable::propertyToString(StyleProperty_e p){
	switch(p){
		default:
			ASSERT(false)
			return "";
		case StyleProperty_e::FILL:
			return "fill";
		case StyleProperty_e::FILL_OPACITY:
			return "fill-opacity";
		case StyleProperty_e::STROKE:
			return "stroke";
		case StyleProperty_e::STROKE_WIDTH:
			return "stroke-width";
		case StyleProperty_e::STROKE_LINECAP:
			return "stroke-linecap";
		case StyleProperty_e::STROKE_LINEJOIN:
			return "stroke-linejoin";
		case StyleProperty_e::STROKE_OPACITY:
			return "stroke-opacity";
		case StyleProperty_e::OPACITY:
			return "opacity";
		case StyleProperty_e::STOP_OPACITY:
			return "stop-opacity";
		case StyleProperty_e::STOP_COLOR:
			return "stop-color";
		case StyleProperty_e::FILL_RULE:
			return "fill-rule";
	}
}
