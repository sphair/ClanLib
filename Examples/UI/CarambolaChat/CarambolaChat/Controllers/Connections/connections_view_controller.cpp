
#include "precomp.h"
#include "connections_view_controller.h"
#include "Views/Connections/network_list_view.h"
#include "Views/Connections/network_list_item_view.h"

using namespace clan;

ConnectionsViewController::ConnectionsViewController()
{
	view->box_style.set_layout_block();
	view->box_style.set_margin(11.0f);

	auto title = std::make_shared<LabelView>();
	title->text_style().set_font("Segoe UI", 20.0f, 25.0f);
	title->text_style().set_weight_bold();
	title->set_text("Chat Networks");
	view->add_subview(title);

	networks = std::make_shared<NetworkListView>();
	view->add_subview(networks);

	for (int i = 0; i < 4; i++)
	{
		auto item = networks->add_item("Freenode");
		if (i % 2 == 0)
			item->set_status_online();
		else
			item->set_status_offline();

	}

}
