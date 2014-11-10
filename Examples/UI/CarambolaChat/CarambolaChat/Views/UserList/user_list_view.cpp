
#include "precomp.h"
#include "user_list_view.h"

using namespace clan;

UserListView::UserListView()
{
	box_style.set_background(Colorf(219, 234, 249));
	box_style.set_padding(7.0f, 5.0f);
	box_style.set_layout_vbox();
}

void UserListView::add_user(const std::string &id, const std::string &name, const std::string &icon)
{
	User user;
	user.id = id;
	user.view = std::make_shared<View>();
	user.view->box_style.set_flex(0.0f, 0.0f);
	user.view->box_style.set_padding(5.0f);
	user.view->box_style.set_layout_hbox();
	user.icon = std::make_shared<ImageView>();
	user.icon->set_image(ImageSource::from_resource(icon));
	user.icon->box_style.set_flex(0.0f, 0.0f);
	user.view->add_subview(user.icon);
	user.label = std::make_shared<LabelView>();
	user.label->set_text(name);
	user.label->text_style().set_color(Colorf::black);
	user.label->text_style().set_font("Segoe UI", 11.0f, 14.0f);
	user.label->box_style.set_margin(5.0f, 0.0f, 0.0f, 0.0f);
	user.view->add_subview(user.label);
	add_subview(user.view);
	users.push_back(user);
}
