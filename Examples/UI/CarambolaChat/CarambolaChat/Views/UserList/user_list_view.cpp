
#include "precomp.h"
#include "user_list_view.h"

using namespace clan;

UserListView::UserListView()
{
	style()->set("background: rgb(219, 234, 249)");
	style()->set("padding: 5px 7px");
	style()->set("flex-direction: column");
}

void UserListView::update_user(const std::string &id, const std::string &name, const std::string &icon)
{
	User &user = users[id];

	if (!user.view)
	{
		user.view = std::make_shared<View>();
		user.view->style()->set("flex: none");
		user.view->style()->set("padding: 5px");
		user.view->style()->set("flex-direction: row");
		user.icon = std::make_shared<ImageView>();
		user.icon->set_image(ImageSource::from_resource(icon));
		user.icon->style()->set("flex: none");
		user.view->add_subview(user.icon);
		user.label = std::make_shared<LabelView>();
		user.label->set_text(name);
		user.label->style()->set("color: black");
		user.label->style()->set("font: 12px/15px 'Source Sans Pro'");
		user.label->style()->set("margin: 0 0 0 5px");
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
