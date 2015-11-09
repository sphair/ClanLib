
#pragma once

class FormEditView : public clan::RowView
{
public:
	FormEditView(const std::string &text)
	{
		label = add_subview<clan::LabelView>();
		textfield = add_subview<clan::TextFieldView>();

		label->set_text(text);
		textfield->set_preferred_size(55);

		style()->set("margin: 3px 0; line-height: 16px");
		label->style()->set("flex: none; width: 100px; margin: auto 0");
		textfield->style()->set("flex: auto");
		textfield->style()->set("border: 1px solid #999; padding: 4px 8px; background: white");
	}

	std::shared_ptr<clan::LabelView> label;
	std::shared_ptr<clan::TextFieldView> textfield;
};

class FormInfoView : public clan::SpanLayoutView
{
public:
	FormInfoView()
	{
		text_style("bold")->set("font-weight: bold");

		add_text("This is a silly model/view/controller example demonstrating of how the ");
		add_text("UI framework", "bold");
		add_text(" can be used with a model class, a controller class and a view tree.");

		style()->set("margin-top: 11px");
		style()->set("flex: auto; width: 200px");
	}
};

class FormView : public clan::ColumnView
{
public:
	FormView()
	{
		name = add_subview<FormEditView>("Name");
		description = add_subview<FormEditView>("Description");
		add_subview<FormInfoView>();

		style()->set("padding: 11px");
		style()->set("background: rgb(240,240,240)");
		style()->set("font: 13px/20px 'Segoe UI'");
	}

	std::shared_ptr<FormEditView> name;
	std::shared_ptr<FormEditView> description;
};
