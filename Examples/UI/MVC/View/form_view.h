
#pragma once

class FormEditView : public clan::RowView
{
public:
	FormEditView(const std::string &text)
	{
		label = add_subview<clan::LabelView>();
		textfield = add_subview<clan::TextFieldView>();

		label->set_text(text);
		textfield->set_preferred_size(50);

		style()->set("margin: 3px 0");
		label->style()->set("flex: none; width: 100px; font: 13px/16px 'Segoe UI'; margin: auto 0");
		textfield->style()->set("flex: auto; font: 13px/16px 'Segoe UI';");
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
		auto normal = std::make_shared<clan::Style>();
		auto bold = std::make_shared<clan::Style>();
		normal->set("font: 13px/16px 'Segoe UI'");
		bold->set("font: bold 13px/16px 'Segoe UI'");

		add_text("This is a silly model/view/controller example demonstrating of how the ", normal);
		add_text("UI framework", bold);
		add_text(" can be used with a model class, a controller class and a view tree.", normal);

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
	}

	std::shared_ptr<FormEditView> name;
	std::shared_ptr<FormEditView> description;
};
