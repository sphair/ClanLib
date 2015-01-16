
#pragma once

class NetworkListItemView;

class NetworkListView : public clan::View
{
public:
	NetworkListView();

	std::shared_ptr<NetworkListItemView> add_item(const std::string &name);

private:
	std::shared_ptr<clan::View> items;
};
