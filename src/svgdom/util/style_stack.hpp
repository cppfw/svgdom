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
