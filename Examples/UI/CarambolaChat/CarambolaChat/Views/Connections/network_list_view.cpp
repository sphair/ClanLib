
#include "precomp.h"
#include "network_list_view.h"
#include "network_list_item_view.h"

using namespace clan;

NetworkListView::NetworkListView()
{
	style()->set("flex-direction: column");

	items_view = std::make_shared<View>();
	items_view->style()->set("flex-direction: column");
	items_view->style()->set("margin: 15px 0");
	items_view->style()->set("background: white"); // Bug: required to make the border work
	items_view->style()->set("border-bottom: 1px solid rgb(204,204,204)");
	add_subview(items_view);

	auto add_button = std::make_shared<ButtonView>();
	add_button->label()->set_text("Add Network");
	add_button->label()->style()->set("font: 13px/16px 'Source Sans Pro'");
	add_button->label()->style()->set("color: rgb(0,0,128)");
	add_subview(add_button);
}

std::shared_ptr<NetworkListItemView> NetworkListView::add_item(const std::string &name)
{
	auto network = std::make_shared<NetworkListItemView>();
	network->set_name(name);
	items_view->add_subview(network);
	items[name] = network;
	return network;
}

std::shared_ptr<NetworkListItemView> NetworkListView::get_item(const std::string &name)
{
	auto it = items.find(name);
	if (it != items.end())
		return it->second;
	else
		return std::shared_ptr<NetworkListItemView>();
}
