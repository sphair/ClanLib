
#include "precomp.h"
#include "user_list_view.h"

using namespace clan;

UserListView::UserListView()
{
	box_style.set_background(Colorf(219, 234, 249));
	box_style.set_padding(7.0f, 5.0f);
	box_style.set_layout_vbox();
}

void UserListView::add_user(const std::string &id, const std::string &name)
{
	User user;
	user.id = id;
	user.view = std::make_shared<LabelView>();
	user.view->set_text(name);
	user.view->box_style.set_flex(0.0f, 0.0f);
	user.view->box_style.set_padding(5.0f);
	user.view->text_style().set_color(Colorf::black);
	user.view->text_style().set_font("Segoe UI", 11.0f, 14.0f);
	add_subview(user.view);
	users.push_back(user);
}
