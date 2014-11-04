
#pragma once

class UserListView : public clan::View
{
public:
	UserListView();

	void add_user(const std::string &id, const std::string &name);

private:
	struct User
	{
		std::string id;
		std::shared_ptr<clan::LabelView> view;
	};

	std::vector<User> users;
};
