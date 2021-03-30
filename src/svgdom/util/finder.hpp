#pragma once

#include <unordered_map>

#include "../elements/element.hpp"

#include "style_stack.hpp"

namespace svgdom{

class finder{
public:
	
	finder(const svgdom::element& root);
	
	struct element_info{
		const svgdom::element& e;
		const style_stack ss;
		
		element_info(const svgdom::element& e, style_stack ss) :
				e(e),
				ss(ss)
		{}
	};

	const element* find_element_by_id(const std::string& id)const;
	const std::list<const element*> find_elements_by_class_name(const std::string& cls)const;
	const std::list<const element*> find_elements_by_tag_name(const std::string& tag_name)const;
	const element_info* find_style_stack_by_id(const std::string& id)const;

	/**
	 * @brief Get element-by-id cache size.
	 * @return number of cached elements.
	 */
	size_t size_of_element_by_id_cache()const noexcept{
		return this->style_stack_by_id_cache.size();
	}

	/**
	 * @brief Get elements-by-class-name cache size.
	 * @return number of cached elements.
	 */
	size_t size_of_elements_by_class_name_cache()const noexcept{
		return this->elements_by_class_name_cache.size();
	}

	/**
	 * @brief Get elements-by-tag-name cache size.
	 * @return number of cached elements.
	 */
	size_t size_of_elements_by_tag_name_cache()const noexcept{
		return this->elements_by_tag_name_cache.size();
	}

	/**
	 * @brief Get style-stack-by-id cache size.
	 * @return number of cached elements.
	 */
	size_t size_of_style_stack_by_id_cache()const noexcept{
		return this->style_stack_by_id_cache.size();
	}

private:
	std::unordered_map<std::string, const element*> element_by_id_cache;
	std::unordered_map<std::string, std::list< const element*>> elements_by_class_name_cache;
	std::unordered_map<std::string, std::list< const element*>> elements_by_tag_name_cache;
	std::unordered_map<std::string, element_info> style_stack_by_id_cache;
};

}
