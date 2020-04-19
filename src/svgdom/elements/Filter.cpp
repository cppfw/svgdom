#include "Filter.hpp"
#include "../visitor.hpp"

using namespace svgdom;

void FilterElement::accept(visitor& visitor){
	visitor.visit(*this);
}

void FilterElement::accept(const_visitor& visitor) const {
	visitor.visit(*this);
}

void FeGaussianBlurElement::accept(visitor& visitor){
	visitor.visit(*this);
}

void FeGaussianBlurElement::accept(const_visitor& visitor) const {
	visitor.visit(*this);
}

void FeColorMatrixElement::accept(visitor& visitor) {
	visitor.visit(*this);
}

void FeBlendElement::accept(visitor& visitor) {
	visitor.visit(*this);
}

void FeCompositeElement::accept(visitor& visitor) {
	visitor.visit(*this);
}

void FeColorMatrixElement::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

void FeBlendElement::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

void FeCompositeElement::accept(const_visitor& visitor) const{
	visitor.visit(*this);
}

std::array<real, 2> FeGaussianBlurElement::getStdDeviation() const noexcept{
	if(!this->isStdDeviationSpecified()){
		return {{0, 0}};
	}
	
	if(this->isStdDeviationYSpecified()){
		return this->stdDeviation;
	}
	
	return {{this->stdDeviation[0], this->stdDeviation[0]}};
}
