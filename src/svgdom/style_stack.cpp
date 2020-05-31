#include "style_stack.hpp"

#include <utki/debug.hpp>

using namespace svgdom;

const svgdom::style_value* style_stack::get_style_property(svgdom::style_property p)const{
	bool explicitInherit = false;

	for (auto i = this->stack.rbegin(); i != this->stack.rend(); ++i) {
		auto v = i->s.get_style_property(p);
		if (!v) {
			v = i->s.get_presentation_attribute(p);
			if(!v){
				if (!explicitInherit && !svgdom::styleable::is_inherited(p)) {
					return nullptr;
				}
				continue;
			}
		}
		if (v->type_ == svgdom::style_value::type::inherit) {
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
	if(!this->ss.stack.empty()){
		ASSERT(this->ss.stack.back().c)
		++this->ss.stack.back().index;
	}

	this->ss.stack.push_back(node(s));
}

style_stack::push::push(style_stack& ss, const svgdom::styleable& s, const svgdom::container& c) :
		push(ss, s)
{
	this->ss.stack.back().c = &c;
}

style_stack::push::~push()noexcept{
	this->ss.stack.pop_back();
}
