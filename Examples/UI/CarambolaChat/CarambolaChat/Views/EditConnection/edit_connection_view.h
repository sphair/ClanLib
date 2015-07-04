
#pragma once

class EditConnectionCheckView : public clan::View
{
public:
	EditConnectionCheckView(const std::string &label_text = std::string())
	{
		value = add_subview<clan::CheckBoxView>();
		label = add_subview<clan::LabelView>();

		label->set_text(label_text);

		style()->set("flex-direction: row");
		style()->set("margin-bottom: 5px");
		label->style()->set("font: 13px/22px 'Source Sans Pro'");
		value->style()->set("font: 13px/22px 'Source Sans Pro'");
		value->style()->set("background: white");
		value->style()->set("width: 16px; height: 16px; margin: auto 0");
		value->style()->set("border: 1px solid #ccc");
		value->style("checked")->set("background: yellow");
	}

	std::shared_ptr<clan::LabelView> label;
	std::shared_ptr<clan::CheckBoxView> value;
};

class EditConnectionTextView : public clan::View
{
public:
	EditConnectionTextView(const std::string &label_text = std::string(), float width = 0.0f)
	{
		label = add_subview<clan::LabelView>();
		value = add_subview<clan::TextFieldView>();

		label->set_text(label_text);

		style()->set("flex-direction: row");
		style()->set("margin-bottom: 5px");
		label->style()->set("width: 150px");
		label->style()->set("font: 13px/22px 'Source Sans Pro'");
		value->style()->set("font: 13px/16px 'Source Sans Pro'");
		value->style()->set("background: white");
		value->style()->set("padding: 2px 5px");
		value->style()->set("border: 1px solid #ccc");

		if (width > 0.0f)
			value->style()->set("width: %1px", width);
		else
			value->style()->set("flex: auto");
	}

	std::shared_ptr<clan::LabelView> label;
	std::shared_ptr<clan::TextFieldView> value;
};

class EditConnectionButtonView : public clan::ButtonView
{
public:
	EditConnectionButtonView(const std::string &text)
	{
		label()->set_text(text);
		label()->set_text_alignment(clan::TextAlignment::center);
		label()->style()->set("font: 13px/16px 'Source Sans Pro'");
		style()->set("background: rgb(245,245,245)");
		style("hot")->set("background: rgb(255,255,255)");
		style()->set("padding: 7px 0; margin-left: 5px");
		style()->set("border: 1px solid #ccc");
		style()->set("width: 75px");
	}
};

class EditConnectionButtonBarView : public clan::View
{
public:
	EditConnectionButtonBarView()
	{
		auto spacer = add_subview();
		ok_button = add_subview<EditConnectionButtonView>("OK");
		cancel_button = add_subview<EditConnectionButtonView>("Cancel");

		style()->set("flex-direction: row");
		style()->set("margin-top: 30px");
		spacer->style()->set("flex: auto");
	}

	std::shared_ptr<EditConnectionButtonView> ok_button;
	std::shared_ptr<EditConnectionButtonView> cancel_button;
};

class EditConnectionView : public clan::View
{
public:
	EditConnectionView()
	{
		connection_name = add_subview<EditConnectionTextView>("Connection name");
		server = add_subview<EditConnectionTextView>("Server");
		port = add_subview<EditConnectionTextView>("Port", 50.0f);
		nick = add_subview<EditConnectionTextView>("Nick", 120.0f);
		alt_nick = add_subview<EditConnectionTextView>("Alternative nick", 120.0f);
		auto_connect = add_subview<EditConnectionCheckView>("Auto connect on startup");
		auto spacer = add_subview();
		button_bar = add_subview<EditConnectionButtonBarView>();

		style()->set("flex-direction: column");
		style()->set("margin: 11px");
		spacer->style()->set("flex: auto");
	}

	std::shared_ptr<EditConnectionTextView> connection_name;
	std::shared_ptr<EditConnectionTextView> server;
	std::shared_ptr<EditConnectionTextView> port;
	std::shared_ptr<EditConnectionTextView> nick;
	std::shared_ptr<EditConnectionTextView> alt_nick;
	std::shared_ptr<EditConnectionCheckView> auto_connect;
	std::shared_ptr<EditConnectionButtonBarView> button_bar;
};
