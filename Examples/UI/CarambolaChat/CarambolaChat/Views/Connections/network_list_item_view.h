
#pragma once

class NetworkListItemView : public clan::View
{
public:
	NetworkListItemView();

	void set_name(const std::string &value) { name->set_text(value); }
	void set_status_online();
	void set_status_offline();

	std::shared_ptr<clan::ButtonView> edit_button;
	std::shared_ptr<clan::ButtonView> remove_button;

private:
	std::shared_ptr<clan::LabelView> name;
	std::shared_ptr<clan::ImageView> status_icon;
	std::shared_ptr<clan::LabelView> status_text;
};
