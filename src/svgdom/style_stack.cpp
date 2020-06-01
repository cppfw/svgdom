#include "style_stack.hpp"

#include <utki/debug.hpp>

#include "casters.hpp"

using namespace svgdom;

style_stack::crawler::crawler(decltype(stack) stack) :
		stack(stack)
{}

const cssdom::styleable& style_stack::crawler::get(){
	ASSERT(!this->stack.empty())
	
	if(this->viter == this->stack.rend()){
		return this->stack.front().s;
	}

	const_styleable_caster c;
	this->hiter->get()->accept(c);

	ASSERT(c.pointer)
	return *c.pointer;
}

bool style_stack::crawler::move_up(){
	TRACE(<< "crawler::move_up(): stack.size() = " << this->stack.size() << std::endl)
	if(this->viter == this->stack.rend()){
		TRACE(<< "crawler::move_up(): at root node" << std::endl)
		return false;
	}
	++this->viter;
	if(this->viter == this->stack.rend()){
		TRACE(<< "crawler::move_up(): came to root node" << std::endl)
		return true;
	}

	TRACE(<< "crawler::move_up(): index = " << this->viter->index << std::endl)
	auto& c = this->viter->c->children;
	ASSERT(!c.empty())
	ASSERT(this->viter->index < c.size())
	this->hiter = std::next(c.begin(), this->viter->index);
	return true;
}

bool style_stack::crawler::move_left(){
	TRACE(<< "crawler::move_left(): invoked" << std::endl)
	if(this->viter == this->stack.rend() || this->hiter == this->viter->c->children.begin()){
		return false;
	}
	auto old_hiter = this->hiter;

	// move left until next stylable is found
	const_styleable_caster c;
	do{
		--this->hiter;
		this->hiter->get()->accept(c);
	}while(!c.pointer);

	if(c.pointer){	
		return true;
	}

	// no styleable found, return to previous position

	this->hiter = old_hiter;
	return false;
}

void style_stack::crawler::reset(){
	TRACE(<< "crawler::reset(): invoked" << std::endl)
	if(this->stack.empty()){
		throw std::logic_error("style_stack::crawler::reset(): stack is empty");
	}

	this->viter = this->stack.rbegin();

	this->move_up();
}

const svgdom::style_value* style_stack::get_style_property(svgdom::style_property p)const{
	bool explicitInherit = false;

	for (auto i = this->stack.rbegin(); i != this->stack.rend(); ++i) {
		auto v = i->s.get_style_property(p);
		if(!v){
			v = this->get_css_style_property(p);
			if(!v){
				v = i->s.get_presentation_attribute(p);
				if(!v){
					if (!explicitInherit && !svgdom::styleable::is_inherited(p)) {
						return nullptr;
					}
					continue;
				}
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
	this->ss.stack.push_back(node(s));
}

style_stack::push::push(style_stack& ss, const svgdom::styleable& s, const svgdom::container& c) :
		push(ss, s)
{
	this->ss.stack.back().c = &c;
}

style_stack::push::~push()noexcept{
	this->ss.stack.pop_back();
	if(!this->ss.stack.empty()){
		ASSERT(this->ss.stack.back().c)
		++this->ss.stack.back().index;
	}
}

void style_stack::add_css(const cssdom::document& css_doc){
	this->css.push_back(css_doc);
}

const style_value* style_stack::get_css_style_property(style_property p)const{
	crawler c(this->stack);
	unsigned specificity = 0;
	const style_value* ret = nullptr;
	for(auto& ss : this->css){
		auto r = ss.get().get_property_value(c, uint32_t(p));
		if(!r.value){
			continue;
		}
		if(r.specificity < specificity){
			continue;
		}
		specificity = r.specificity;
		ret = &static_cast<const style_element::css_style_value*>(r.value)->value;
	}
	return ret;
}
