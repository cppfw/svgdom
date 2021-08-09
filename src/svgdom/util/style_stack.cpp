/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#include "style_stack.hpp"

#include <utki/debug.hpp>

#include "casters.hpp"

using namespace svgdom;

style_stack::crawler::crawler(decltype(stack) stack) :
		stack(stack)
{}

const cssom::styleable& style_stack::crawler::get(){
	ASSERT(!this->stack.empty())
	
	return this->iter->get();
}

bool style_stack::crawler::move_up(){
	// TRACE(<< "crawler::move_up(): stack.size() = " << this->stack.size() << std::endl)
	if(std::distance(this->iter, this->stack.rend()) == 1){
		// TRACE(<< "crawler::move_up(): at root node" << std::endl)
		return false;
	}
	++this->iter;
	return true;
}

bool style_stack::crawler::move_left(){
	// TRACE(<< "crawler::move_left(): NOT SUPPORTED!!!" << std::endl)
	return false;
}

void style_stack::crawler::reset(){
	// TRACE(<< "crawler::reset(): invoked" << std::endl)
	if(this->stack.empty()){
		throw std::logic_error("style_stack::crawler::reset(): stack is empty");
	}

	this->iter = this->stack.rbegin();
}

const svgdom::style_value* style_stack::get_style_property(svgdom::style_property p)const{
	bool explicit_inherit = false;

	for(auto i = this->stack.rbegin(); i != this->stack.rend(); ++i){
		auto v = i->get().get_style_property(p);
		if(!v){
			v = this->get_css_style_property(p);
			if(!v){
				v = i->get().get_presentation_attribute(p);
				if(!v){
					if(!explicit_inherit && !svgdom::styleable::is_inherited(p)){
						return nullptr;
					}
					continue;
				}
			}
		}
		ASSERT(v)
		if(is_inherit(*v)){
			explicit_inherit = true;
			continue;
		}
		return v;
	}

	return nullptr;
}

style_stack::push::push(style_stack& ss, const svgdom::styleable& s) :
		ss(ss)
{
	this->ss.stack.push_back(s);
}

style_stack::push::~push()noexcept{
	this->ss.stack.pop_back();
}

void style_stack::add_css(const cssom::sheet& css_doc){
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
