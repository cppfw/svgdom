#include "Filter.hpp"
#include "../Visitor.hpp"


using namespace svgdom;

void FilterElement::accept(Visitor& visitor){
	visitor.visit(*this);
}

void FilterElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void FeGaussianBlurElement::accept(Visitor& visitor){
	visitor.visit(*this);
}

void FeGaussianBlurElement::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void FeColorMatrixElement::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void FeBlendElement::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void FeCompositeElement::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void FeColorMatrixElement::accept(ConstVisitor& visitor) const{
	visitor.visit(*this);
}

void FeBlendElement::accept(ConstVisitor& visitor) const{
	visitor.visit(*this);
}

void FeCompositeElement::accept(ConstVisitor& visitor) const{
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
