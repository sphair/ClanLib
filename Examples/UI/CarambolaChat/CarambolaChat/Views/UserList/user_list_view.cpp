
#include "precomp.h"
#include "user_list_view.h"

using namespace clan;

UserListView::UserListView()
{
	box_style.set_background(Colorf(219, 234, 249));
	box_style.set_padding(7.0f, 5.0f);
	box_style.set_layout_vbox();
}

void UserListView::update_user(const std::string &id, const std::string &name, const std::string &icon)
{
	User &user = users[id];

	if (!user.view)
	{
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
		user.label->text_style().set_font("Segoe UI", 12.0f, 15.0f);
		user.label->box_style.set_margin(5.0f, 0.0f, 0.0f, 0.0f);
		user.view->add_subview(user.label);
		add_subview(user.view);
	}

	user.label->set_text(name);
	user.icon->set_image(ImageSource::from_resource(icon));
}

void UserListView::remove_user(const std::string &id)
{
	auto it = users.find(id);
	if (it != users.end())
	{
		it->second.view->remove_from_super();
		users.erase(it);
	}
}

void UserListView::rename_user(const std::string &old_id, const std::string &new_id, const std::string &name)
{
	if (!has_user(old_id))
		throw Exception("Unknown user id");

	if (old_id != new_id)
	{
		users[new_id] = users[old_id];
		users.erase(users.find(old_id));
	}

	users[new_id].label->set_text(name);
}

void UserListView::sort()
{
}

void UserListView::clear()
{
	for (auto &it : users)
	{
		it.second.view->remove_from_super();
	}
	users.clear();
}
