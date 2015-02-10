
#include "precomp.h"
#include "network_list_item_view.h"

using namespace clan;

NetworkListItemView::NetworkListItemView()
{
	box_style.set_layout_hbox();
	box_style.set_padding(0.0f, 5.0f);
	box_style.set_background(Colorf::white); // Bug: required to make the border work
	box_style.set_border(Colorf::gray80, 0.0f, 1.0f, 0.0f, 0.0f);

	status_icon = std::make_shared<ImageView>();
	status_icon->box_style.set_flex(0.0f, 0.0f);
	status_icon->box_style.set_width(32.0f);
	status_icon->box_style.set_margin(7.0f, 0.0f, 7.0f, 0.0);
	add_subview(status_icon);

	auto name_status_block = std::make_shared<View>();
	name_status_block->box_style.set_flex(0.0f, 0.0f);
	name_status_block->box_style.set_width(220.0f);
	name_status_block->box_style.set_layout_vbox();

	name = std::make_shared<LabelView>();
	name->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	name->text_style().set_weight_bold();
	name_status_block->add_subview(name);

	status_text = std::make_shared<LabelView>();
	status_text->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	name_status_block->add_subview(status_text);

	add_subview(name_status_block);

	connect_button = std::make_shared<ButtonView>();
	connect_button->label()->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	connect_button->label()->text_style().set_color(Colorf::navy);
	connect_button->box_style.set_flex(0.0f, 0.0f);
	connect_button->box_style.set_margin(0.0f, 0.0f, 15.0f, 0.0);
	connect_button->box_style.set_margin_top_auto();
	connect_button->box_style.set_margin_bottom_auto();
	add_subview(connect_button);

	edit_button = std::make_shared<ButtonView>();
	edit_button->label()->set_text("Edit");
	edit_button->label()->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	edit_button->label()->text_style().set_color(Colorf::navy);
	edit_button->box_style.set_flex(0.0f, 0.0f);
	edit_button->box_style.set_margin(0.0f, 0.0f, 15.0f, 0.0);
	edit_button->box_style.set_margin_top_auto();
	edit_button->box_style.set_margin_bottom_auto();
	add_subview(edit_button);

	remove_button = std::make_shared<ButtonView>();
	remove_button->label()->set_text("Remove");
	remove_button->label()->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	remove_button->label()->text_style().set_color(Colorf::navy);
	remove_button->box_style.set_flex(0.0f, 0.0f);
	remove_button->box_style.set_margin_top_auto();
	remove_button->box_style.set_margin_bottom_auto();
	add_subview(remove_button);

	set_status_offline("Not Connected");
}

void NetworkListItemView::set_status_online(const std::string &text)
{
	status_icon->set_image(ImageSource::from_resource("Icons/status_online32.png"));
	status_text->set_text(text);
	connect_button->label()->set_text("Disconnect");
}

void NetworkListItemView::set_status_offline(const std::string &text)
{
	status_icon->set_image(ImageSource::from_resource("Icons/status_offline32.png"));
	status_text->set_text(text);
	connect_button->label()->set_text("Connect");
}
