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

#pragma once

#include <vector>

#include <cssom/om.hpp>

#include "../elements/styleable.hpp"
#include "../elements/container.hpp"

namespace svgdom{
class style_stack{
public:
	std::vector<std::reference_wrapper<const styleable>> stack;

private:
	std::vector<std::reference_wrapper<const cssom::sheet>> css;

	class crawler : public cssom::xml_dom_crawler{
		const decltype(style_stack::stack)& stack;

		std::remove_reference<decltype(stack)>::type::const_reverse_iterator iter;

	public:
		crawler(decltype(stack) stack);

		const cssom::styleable& get()override;

		bool move_up()override;
		bool move_left()override;
		void reset()override;
	};

	const svgdom::style_value* get_css_style_property(svgdom::style_property p)const;
public:
	const svgdom::style_value* get_style_property(svgdom::style_property p)const;
	
	void add_css(const cssom::sheet& css_doc);

	class push{
		style_stack& ss;
	public:
		push(style_stack& ss, const svgdom::styleable& s);
		~push()noexcept;
	};
};

}
