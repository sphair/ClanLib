
#include "precomp.h"
#include "network_list_item_view.h"

using namespace clan;

NetworkListItemView::NetworkListItemView()
{
	style()->set("flex-direction: row");
	style()->set("padding: 5px 0");
	style()->set("background: white"); // Bug: required to make the border work
	style()->set("border-top: 1px solid rgb(204,204,204)");

	status_icon = std::make_shared<ImageView>();
	status_icon->style()->set("flex: 0 0 main-size");
	status_icon->style()->set("width: 32px");
	status_icon->style()->set("margin: 0 7px");
	add_subview(status_icon);

	auto name_status_block = std::make_shared<View>();
	name_status_block->style()->set("flex: 0 0 main-size");
	name_status_block->style()->set("width: 220px");
	name_status_block->style()->set("flex-direction: column");

	name = std::make_shared<LabelView>();
	name->style()->set("font: bold 13px/16px 'Source Sans Pro'");
	name_status_block->add_subview(name);

	status_text = std::make_shared<LabelView>();
	status_text->style()->set("font: 13px/16px 'Source Sans Pro'");
	name_status_block->add_subview(status_text);

	add_subview(name_status_block);

	connect_button = std::make_shared<ButtonView>();
	connect_button->label()->style()->set("font: 13px/16px 'Source Sans Pro'");
	connect_button->label()->style()->set("color: rgb(0,0,128)");
	connect_button->style()->set("flex: 0 0 main-size");
	connect_button->style()->set("margin: auto 15px auto 0");
	add_subview(connect_button);

	edit_button = std::make_shared<ButtonView>();
	edit_button->label()->set_text("Edit");
	edit_button->label()->style()->set("font: 13px/16px 'Source Sans Pro'");
	edit_button->label()->style()->set("color: rgb(0,0,128)");
	edit_button->style()->set("flex: 0 0 main-size");
	edit_button->style()->set("margin: auto 15px auto 0");
	add_subview(edit_button);

	remove_button = std::make_shared<ButtonView>();
	remove_button->label()->set_text("Remove");
	remove_button->label()->style()->set("font: 13px/16px 'Source Sans Pro'");
	remove_button->label()->style()->set("color: rgb(0,0,128)");
	remove_button->style()->set("flex: 0 0 main-size");
	remove_button->style()->set("margin: auto 0");
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
