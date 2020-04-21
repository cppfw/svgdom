#include "style_stack.hpp"

#include <utki/debug.hpp>

using namespace svgdom;

const svgdom::style_value* style_stack::get_style_property(svgdom::style_property p)const{
	bool explicitInherit = false;

	for (auto i = this->stack.rbegin(); i != this->stack.rend(); ++i) {
		ASSERT(*i)
		auto v = (*i)->findStyleProperty(p);
		if (!v) {
			if (!explicitInherit && !svgdom::styleable::is_inherited(p)) {
				return nullptr;
			}
			continue;
		}
		if (v->type == svgdom::style_value_type::INHERIT) {
			explicitInherit = true;
			continue;
		}
		return v;
	}

	return nullptr;
}

style_stack::push::push(style_stack& ss, const svgdom::styleable& s) :
		ss(ss)
{
	this->ss.stack.push_back(&s);
}

style_stack::push::~push()noexcept{
	this->ss.stack.pop_back();
}
