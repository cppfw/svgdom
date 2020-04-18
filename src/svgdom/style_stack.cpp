#include "style_stack.hpp"

#include <utki/debug.hpp>

using namespace svgdom;

const svgdom::StyleValue* style_stack::get_style_property(svgdom::StyleProperty_e p)const{
	bool explicitInherit = false;

	for (auto i = this->stack.rbegin(); i != this->stack.rend(); ++i) {
		ASSERT(*i)
		auto v = (*i)->findStyleProperty(p);
		if (!v) {
			if (!explicitInherit && !svgdom::Styleable::isStylePropertyInherited(p)) {
				return nullptr;
			}
			continue;
		}
		if (v->type == svgdom::StyleValue::Type_e::INHERIT) {
			explicitInherit = true;
			continue;
		}
		return v;
	}

	return nullptr;
}

style_stack::push::push(style_stack& ss, const svgdom::Styleable& s) :
		ss(ss)
{
	this->ss.stack.push_back(&s);
}

style_stack::push::~push()noexcept{
	this->ss.stack.pop_back();
}
