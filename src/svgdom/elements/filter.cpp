#include "filter.hpp"
#include "../visitor.hpp"

using namespace svgdom;

const std::string filter_element::tag = "filter";
const std::string fe_gaussian_blur_element::tag = "feGaussianBlur";
const std::string fe_color_matrix_element::tag = "feColorMatrix";
const std::string fe_blend_element::tag = "feBlend";
const std::string fe_composite_element::tag = "feComposite";

void filter_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void filter_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_blend_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_composite_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_blend_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

void fe_composite_element::accept(const_visitor& visitor)const{
	visitor.visit(*this);
}

r4::vector2<real> fe_gaussian_blur_element::get_std_deviation()const noexcept{
	if(!this->is_std_deviation_specified()){
		return 0;
	}
	
	if(this->is_std_deviation_y_specified()){
		return this->std_deviation;
	}
	
	return this->std_deviation.x();
}
