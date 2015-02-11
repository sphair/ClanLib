
#pragma once

class UserListView : public clan::View
{
public:
	UserListView();

	void update_user(const std::string &id, const std::string &name, const std::string &icon);
	void remove_user(const std::string &id);
	void rename_user(const std::string &old_id, const std::string &new_id, const std::string &name);
	bool has_user(const std::string &id) const { return users.find(id) != users.end(); }
	void sort();
	void clear();

private:
	struct User
	{
		std::shared_ptr<clan::View> view;
		std::shared_ptr<clan::ImageView> icon;
		std::shared_ptr<clan::LabelView> label;
	};

	std::map<std::string, User> users;
};
