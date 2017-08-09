#include "Filter.hpp"
#include "../Visitor.hpp"


using namespace svgdom;

void FilterElement::accept(Visitor& visitor) const {
	visitor.visit(*this);
}

void FeGaussianBlurElement::accept(Visitor& visitor) const {
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
