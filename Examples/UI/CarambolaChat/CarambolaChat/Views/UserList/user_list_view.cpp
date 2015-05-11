
#include "precomp.h"
#include "user_list_view.h"

using namespace clan;

UserListView::UserListView()
{
	style()->set("background: rgb(219, 234, 249)");
	content_view()->style()->set("padding: 5px 7px");
	content_view()->style()->set("flex-direction: column");
	scrollbar_y_view()->style()->set("flex: none");
	scrollbar_y_view()->style()->set("background: rgb(232,232,236)");
	scrollbar_y_view()->track()->style()->set("padding: 0 4px");
	scrollbar_y_view()->thumb()->style()->set("background: rgb(208,209,215)");
}

void UserListView::update_user(const std::string &id, const std::string &name, const std::string &icon, int sort_priority)
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
		content_view()->add_subview(user.view);
	}

	user.sort_priority = sort_priority;
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
	std::vector<std::string> keys;
	for (auto &it : users)
		keys.push_back(it.first);

	std::stable_sort(keys.begin(), keys.end(), [this](const std::string &key1, const std::string &key2)
	{
		const auto &value1 = users.find(key1)->second;
		const auto &value2 = users.find(key2)->second;
		if (value1.sort_priority != value2.sort_priority)
			return value1.sort_priority < value2.sort_priority;
		else
			return StringHelp::compare(value1.label->text(), value2.label->text(), true) < 0;
	});

	for (const auto &key : keys)
	{
		auto &value = users[key];
		value.view->remove_from_super();
		content_view()->add_subview(value.view);
	}
}

void UserListView::clear()
{
	for (auto &it : users)
	{
		it.second.view->remove_from_super();
	}
	users.clear();
}
