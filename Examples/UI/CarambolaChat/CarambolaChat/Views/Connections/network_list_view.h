
#pragma once

class NetworkListItemView;

class NetworkListView : public clan::View
{
public:
	NetworkListView();

	std::shared_ptr<NetworkListItemView> add_item(const std::string &name);
	std::shared_ptr<NetworkListItemView> get_item(const std::string &name);

private:
	std::shared_ptr<clan::View> items_view;
	std::map<std::string, std::shared_ptr<NetworkListItemView>> items;
};
