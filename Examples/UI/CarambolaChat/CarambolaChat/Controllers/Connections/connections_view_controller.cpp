
#include "precomp.h"
#include "connections_view_controller.h"

using namespace clan;

ConnectionsViewController::ConnectionsViewController()
{
	view->box_style.set_layout_block();
	view->box_style.set_margin(11.0f);

	auto title = std::make_shared<LabelView>();
	title->text_style().set_font("Segoe UI", 20.0f, 25.0f);
	title->text_style().set_weight_bold();
	title->set_text("Chat Networks");
	view->add_subview(title);

	auto networks = std::make_shared<View>();
	networks->box_style.set_layout_block();
	networks->box_style.set_margin(0.0f, 15.0f);
	networks->box_style.set_background(Colorf::white); // Bug: required to make the border work
	networks->box_style.set_border(Colorf::gray80, 0.0f, 0.0f, 0.0f, 1.0f);
	view->add_subview(networks);

	for (int i = 0; i < 4; i++)
	{
		auto network = std::make_shared<View>();
		network->box_style.set_layout_hbox();
		network->box_style.set_padding(0.0f, 5.0f);
		network->box_style.set_background(Colorf::white); // Bug: required to make the border work
		network->box_style.set_border(Colorf::gray80, 0.0f, 1.0f, 0.0f, 0.0f);

		auto status_icon = std::make_shared<ImageView>();
		status_icon->box_style.set_flex(0.0f, 0.0f);
		status_icon->box_style.set_width(32.0f);
		status_icon->box_style.set_margin(7.0f, 0.0f, 7.0f, 0.0);
		if (i % 2 == 0)
			status_icon->set_image(ImageSource::from_resource("Icons/status_offline32.png"));
		else
			status_icon->set_image(ImageSource::from_resource("Icons/status_online32.png"));
		network->add_subview(status_icon);

		auto name_status_block = std::make_shared<View>();
		name_status_block->box_style.set_flex(0.0f, 0.0f);
		name_status_block->box_style.set_width(220.0f);
		name_status_block->box_style.set_layout_vbox();

		auto name = std::make_shared<LabelView>();
		name->set_text("Freenode");
		name->text_style().set_font("Segoe UI", 13.0f, 16.0f);
		name->text_style().set_weight_bold();
		name_status_block->add_subview(name);

		auto status_text = std::make_shared<LabelView>();
		status_text->text_style().set_font("Segoe UI", 13.0f, 16.0f);
		status_text->set_text("Not connected");
		name_status_block->add_subview(status_text);

		network->add_subview(name_status_block);

		auto edit_button = std::make_shared<ButtonView>();
		edit_button->label()->set_text("Edit");
		edit_button->label()->text_style().set_font("Segoe UI", 13.0f, 16.0f);
		edit_button->label()->text_style().set_color(Colorf::navy);
		edit_button->box_style.set_flex(0.0f, 0.0f);
		edit_button->box_style.set_margin(0.0f, 0.0f, 15.0f, 0.0);
		edit_button->box_style.set_margin_top_auto();
		edit_button->box_style.set_margin_bottom_auto();
		network->add_subview(edit_button);

		auto remove_button = std::make_shared<ButtonView>();
		remove_button->label()->set_text("Remove");
		remove_button->label()->text_style().set_font("Segoe UI", 13.0f, 16.0f);
		remove_button->label()->text_style().set_color(Colorf::navy);
		remove_button->box_style.set_flex(0.0f, 0.0f);
		remove_button->box_style.set_margin_top_auto();
		remove_button->box_style.set_margin_bottom_auto();
		network->add_subview(remove_button);

		networks->add_subview(network);
	}

	auto add_button = std::make_shared<ButtonView>();
	add_button->label()->set_text("Add Network");
	add_button->label()->text_style().set_font("Segoe UI", 13.0f, 16.0f);
	add_button->label()->text_style().set_color(Colorf::navy);
	view->add_subview(add_button);

}
