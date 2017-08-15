#include "../../src/svgdom/dom.hpp"
#include "../../src/svgdom/StreamWriter.hpp"

#include <utki/debug.hpp>


//Visitor to remove all Line elements
class EditingVisitor : public svgdom::Visitor{
	std::vector<std::pair<svgdom::Container*, T_ChildIter>> elementsToRemove;
	
	void addToRemove(){
		if(this->curParent()){
			//root element does not have parent, nowhere to remove it from
			return;
		}
		this->elementsToRemove.push_back(std::make_pair(this->curParent(), this->curIter()));
	}
	
public:
	void visit(svgdom::SvgElement& e) override{
		this->relayAccept(e);
	}
	
	void visit(svgdom::PathElement& e) override{
		e.id = "new id for path";
	}

	void visit(svgdom::LineElement& e) override{
		this->addToRemove();
	}
	
	void removeLines(){
		for(auto& p : this->elementsToRemove){
			ASSERT_ALWAYS(p.first)
			p.first->children.erase(p.second);
		}
	}
};

int main(int argc, char** argv){
	auto dom = utki::makeUnique<svgdom::SvgElement>();

	svgdom::PathElement path;

	svgdom::PathElement::Step step;

	step.type = svgdom::PathElement::Step::Type_e::MOVE_ABS;
	step.x = 0;
	step.y = 0;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 0;
	step.y = 300;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 300;
	path.path.push_back(step);

	step.type = svgdom::PathElement::Step::Type_e::LINE_ABS;
	step.x = 300;
	step.y = 0;
	path.path.push_back(step);

	dom->children.push_back(utki::makeUnique<svgdom::PathElement>(path));

	dom->children.push_back(utki::makeUnique<svgdom::LineElement>());
	
	//TODO: add more lines
	
	EditingVisitor visitor;
	
	dom->accept(visitor);
	
	//TODO: assert
	
//	ASSERT_ALWAYS(str.find("xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"") != std::string::npos)
//	ASSERT_ALWAYS(str.find("<custom customAttrib1=\"value1\" customAttrib2=\"value2\"/>") != std::string::npos)
}
