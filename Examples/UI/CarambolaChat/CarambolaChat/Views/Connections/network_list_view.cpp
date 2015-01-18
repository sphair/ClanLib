
#include "precomp.h"
#include "network_list_view.h"
#include "network_list_item_view.h"

using namespace clan;

NetworkListView::NetworkListView()
{
	items_view = std::make_shared<View>();
	items_view->box_style.set_layout_block();
	items_view->box_style.set_margin(0.0f, 15.0f);
	items_view->box_style.set_background(Colorf::white); // Bug: required to make the border work
	items_view->box_style.set_border(Colorf::gray80, 0.0f, 0.0f, 0.0f, 1.0f);
	add_subview(items_view);

	auto add_button = std::make_shared<ButtonView>();
	add_button->label()->set_text("Add Network");
	add_button->label()->text_style().set_font("Source Sans Pro", 13.0f, 16.0f);
	add_button->label()->text_style().set_color(Colorf::navy);
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
