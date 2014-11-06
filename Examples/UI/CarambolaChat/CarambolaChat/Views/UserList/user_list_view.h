
#pragma once

class UserListView : public clan::View
{
public:
	UserListView();

	void add_user(const std::string &id, const std::string &name, const std::string &icon);

private:
	struct User
	{
		std::string id;
		std::shared_ptr<clan::View> view;
		std::shared_ptr<clan::ImageView> icon;
		std::shared_ptr<clan::LabelView> label;
	};

	std::vector<User> users;
};
