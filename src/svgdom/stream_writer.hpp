#pragma once

#include "visitor.hpp"

namespace svgdom{

class stream_writer : virtual public const_visitor{
private:
	void childrenToStream(const container& e);
	
	std::string name;
	std::vector<std::pair<std::string, std::string>> attributes;
protected:
	// s, indent, and indentStr() are made protected to allow writing arbitrary content to stream for those who extend the class, as this was needed in some projects.
	std::ostream& s;
	unsigned indent = 0;
	std::string indent_str();
	
	void set_name(const std::string& name);
	void add_attribute(const std::string& name, const std::string& value);
	void add_attribute(const std::string& name, const length& value);
	void add_attribute(const std::string& name, real value);
	void write(const container* children = nullptr);
	
	void add_element_attributes(const element& e);
	void add_transformable_attributes(const Transformable& e);
	void add_styleable_attributes(const styleable& e);
	void add_view_boxed_attributes(const ViewBoxed& e);
	void add_aspect_ratioed_attributes(const aspect_ratioed& e);
	void add_rectangle_attributes(
			const rectangle& e,
			const rectangle& default_values = rectangle(
					length(0, length::Unit_e::PERCENT),
					length(0, length::Unit_e::PERCENT),
					length(100, length::Unit_e::PERCENT),
					length(100, length::Unit_e::PERCENT)
				)
		);
	void add_shape_attributes(const shape& e);
	void add_referencing_attributes(const referencing& e);
	void add_gradient_attributes(const gradient& e);
	void add_filter_primitive_attributes(const FilterPrimitive& e);
	void add_inputable_attributes(const Inputable& e);
	void add_second_inputable_attributes(const SecondInputable& e);
	void add_text_positioning_attributes(const TextPositioning& e);
	
	// TODO: deprecated, remove.
	std::string indentStr(){
		return this->indent_str();
	}

	// TODO: DEPRECATED, remove.
	void setName(const std::string& name){
		this->set_name(name);
	}

	// TODO: DEPRECATED, remove.
	void addAttribute(const std::string& name, const std::string& value){
		this->add_attribute(name, value);
	}

	// TODO: deprecaed, remove.
	void addAttribute(const std::string& name, const length& value){
		this->add_attribute(name, value);
	}

	// TODO: deprecated, remove.
	void addAttribute(const std::string& name, real value){
		this->add_attribute(name, value);
	}

	// TODO: deprecatged, remove.
	void addElementAttributes(const element& e){
		this->add_element_attributes(e);
	}

	// TODO: deprecated, remove.
	void addTransformableAttributes(const Transformable& e){
		this->add_transformable_attributes(e);
	}

	// TODO: deprecated, remove.
	void addStyleableAttributes(const styleable& e){
		this->add_styleable_attributes(e);
	}

	// TODO: deprecated, remove.
	void addViewBoxedAttributes(const ViewBoxed& e){
		this->add_view_boxed_attributes(e);
	}

	// TODO: deprecated, remove.
	void addAspectRatioedAttributes(const aspect_ratioed& e){
		this->add_aspect_ratioed_attributes(e);
	}

	// TODO: deprecaed, remove.
	void addRectangleAttributes(
			const rectangle& e,
			const rectangle& defaultValues = rectangle(
					length(0, length::Unit_e::PERCENT),
					length(0, length::Unit_e::PERCENT),
					length(100, length::Unit_e::PERCENT),
					length(100, length::Unit_e::PERCENT)
				)
		)
	{
		this->add_rectangle_attributes(e, defaultValues);
	}

	// TODO: deprecated, remove.
	void addShapeAttributes(const shape& e){
		this->add_shape_attributes(e);
	}

	// TODO: deprecated, remove.
	void addReferencingAttributes(const referencing& e){
		this->add_referencing_attributes(e);
	}

	// TODO: deprecated, remove.
	void addGradientAttributes(const gradient& e){
		this->add_gradient_attributes(e);
	}

	// TODO: deprecated, remove.
	void addFilterPrimitiveAttributes(const FilterPrimitive& e){
		this->add_filter_primitive_attributes(e);
	}

	// TODO: deprecated, remove.
	void addInputableAttributes(const Inputable& e){
		this->add_inputable_attributes(e);
	}

	// TODO: deprecated, remove.
	void addSecondInputableAttributes(const SecondInputable& e){
		this->add_second_inputable_attributes(e);
	}

	// TODO: deprecated, remove.
	void addTextPositioningAttributes(const TextPositioning& e){
		this->add_text_positioning_attributes(e);
	}
public:
	stream_writer(std::ostream& s) : s(s) {}
	
	void visit(const g_element& e) override;
	void visit(const svg_element& e) override;
	void visit(const symbol_element& e) override;
	void visit(const defs_element& e) override;
	void visit(const linear_gradient_element& e) override;
	void visit(const radial_gradient_element& e) override;
	void visit(const gradient::stop_element& e) override;
	void visit(const use_element& e) override;
	void visit(const path_element& e) override;
	void visit(const circle_element& e) override;
	void visit(const polyline_element& e) override;
	void visit(const polygon_element& e) override;
	void visit(const ellipse_element& e) override;
	void visit(const rect_element& e) override;
	void visit(const line_element& e) override;
	void visit(const FilterElement& e) override;
	void visit(const FeGaussianBlurElement& e) override;
	void visit(const FeColorMatrixElement& e) override;
	void visit(const FeBlendElement& e) override;
	void visit(const FeCompositeElement& e) override;
	void visit(const image_element& e) override;
	void visit(const mask_element& e) override;
	void visit(const TextElement& e) override;
};

// TODO: deprecated, remove.
typedef stream_writer StreamWriter;

}
