
#include "precomp.h"
#include "chat_window_controller.h"

using namespace clan;

ChatWindowViewController::ChatWindowViewController()
{
	DisplayWindowDescription desc;
	desc.set_title("");
	desc.set_allow_resize(true);
	desc.set_size(Size(1536, 864), false);
	desc.set_type(WindowType::custom);
	desc.set_extend_frame(8, 46, 8, 8);

	view = std::make_shared<WindowView>(desc);
	window_view()->show(WindowShowType::show_default);

	view->box_style.set_layout_vbox();

	auto tab_labels = std::make_shared<View>();
	tab_labels->box_style.set_flex(0.0f, 0.0f);
	tab_labels->box_style.set_padding(7.0f, 14.0f, 8.0f, 0.0f);
	tab_labels->box_style.set_height(32.0f);
	tab_labels->box_style.set_layout_hbox();

	const char *names[] =
	{
		"Freenode",
		"#clanlib",
		"#opengl"
	};

	for (int i = 0; i < 3; i++)
	{
		auto tab_label = std::make_shared<View>();
		tab_label->box_style.set_flex(0.0f, 1.0f);
		if (i == 1)
		{
			tab_label->box_style.set_background_gradient_to_bottom(Colorf::white, Colorf(235, 243, 252));
			tab_label->box_style.set_border(Colorf::gray40, 1.0f, 1.0f, 1.0f, 0.0f);
		}
		else
		{
			tab_label->box_style.set_border(Colorf::transparent, 1.0f, 1.0f, 1.0f, 0.0f);
		}
		tab_label->box_style.set_border_radius(6.0f, 6.0f, 0.0f, 0.0f);
		tab_label->box_style.set_padding(10.0f, 2.0f);
		tab_label->box_style.set_margin(5.0f, 0.0f);
		tab_labels->add_subview(tab_label);

		auto label = std::make_shared<LabelView>();
		label->text_style().set_font("Segoe UI", 12.0f, 28.0f);
		label->text_style().set_target_transparent();
		label->set_text(names[i]);
		tab_label->add_subview(label);
	}

	auto tab_toolbar = std::make_shared<View>();
	tab_toolbar->box_style.set_flex(0.0f, 0.0f);
	tab_toolbar->box_style.set_height(40.0f);
	tab_toolbar->box_style.set_margin(8.0f, 0.0f);
	tab_toolbar->box_style.set_background_gradient_to_bottom(Colorf(235, 243, 252), Colorf(219, 234, 249));
	tab_toolbar->box_style.set_border(Colorf::gray60, 0.0f, 0.0f, 0.0f, 1.0f);

	auto tab_page = std::make_shared<View>();
	tab_page->box_style.set_flex(1.0f, 1.0f);
	tab_page->box_style.set_margin(8, 0, 8, 8);
	tab_page->box_style.set_background(Colorf::white);
	tab_page->box_style.set_layout_vbox();

	auto chat_users_group = std::make_shared<View>();
	chat_users_group->box_style.set_layout_hbox();
	chat_users_group->box_style.set_flex(1.0f, 1.0f);

	auto chat_log = std::make_shared<View>();
	chat_log->box_style.set_flex(1.0f, 1.0f);
	chat_users_group->add_subview(chat_log);

	auto users_list = std::make_shared<View>();
	users_list->box_style.set_background(Colorf(219, 234, 249));
	users_list->box_style.set_width(200.0f);
	users_list->box_style.set_flex(0.0f, 0.0f);
	chat_users_group->add_subview(users_list);

	auto input_bar = std::make_shared<View>();
	input_bar->box_style.set_flex(0.0f, 0.0f);
	input_bar->box_style.set_background_gradient_to_bottom(Colorf(235, 243, 252), Colorf(219, 234, 249));
	input_bar->box_style.set_border(Colorf::gray60, 0.0f, 1.0f, 0.0f, 0.0f);
	input_bar->box_style.set_padding(5.0f);
	input_bar->box_style.set_layout_hbox();

	auto input_text = std::make_shared<TextFieldView>();
	input_text->box_style.set_border(Colorf::gray60, 1.0f);
	input_text->box_style.set_border_radius(2.0f);
	input_text->box_style.set_background(Colorf::white);
	input_text->box_style.set_padding(5.0f, 2.0f);
	input_text->text_style().set_font("Segoe UI", 12.0f, 20.0f);
	input_text->set_text("This is the amazing text input field!");
	input_bar->add_subview(input_text);

	tab_page->add_subview(chat_users_group);
	tab_page->add_subview(input_bar);

	view->add_subview(tab_labels);
	view->add_subview(tab_toolbar);
	view->add_subview(tab_page);
}
