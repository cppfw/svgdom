#include "filter.hpp"
#include "../visitor.hpp"

using namespace svgdom;

void filter_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void filter_element::accept(const_visitor& visitor) const {
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(visitor& visitor){
	visitor.visit(*this);
}

void fe_gaussian_blur_element::accept(const_visitor& visitor) const {
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(visitor& visitor) {
	visitor.visit(*this);
}

void FeBlendElement::accept(visitor& visitor) {
	visitor.visit(*this);
}

void fe_composite_element::accept(visitor& visitor) {
	visitor.visit(*this);
}

void fe_color_matrix_element::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

void FeBlendElement::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

void fe_composite_element::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

std::array<real, 2> fe_gaussian_blur_element::get_std_deviation() const noexcept{
	if(!this->is_std_deviation_specified()){
		return {{0, 0}};
	}
	
	if(this->is_std_deviation_y_specified()){
		return this->stdDeviation;
	}
	
	return {{this->stdDeviation[0], this->stdDeviation[0]}};
}
