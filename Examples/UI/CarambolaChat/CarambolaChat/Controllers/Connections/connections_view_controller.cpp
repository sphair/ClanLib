
#include "precomp.h"
#include "connections_view_controller.h"
#include "Views/Connections/network_list_view.h"
#include "Views/Connections/network_list_item_view.h"
#include "Models/XMLSettings/xml_settings_list.h"
#include "Models/app_model.h"

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

	XMLSettingsList connections = AppModel::instance()->settings.xml_settings.get_list("connections");
	int connection_count = connections.get_count();
	for (int i = 0; i < connection_count; ++i)
	{
		XMLSettings connection = connections.get(i);
		std::string name = connection.get_string("connectionname");

		auto item = networks->add_item(name);
		item->set_status_offline();

		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::at_target), [=](PointerEvent &) { connect_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::bubbling), [=](PointerEvent &) { connect_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::at_target), [=](PointerEvent &) { edit_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::bubbling), [=](PointerEvent &) { edit_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::at_target), [=](PointerEvent &) { remove_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(EventUIPhase::bubbling), [=](PointerEvent &) { remove_clicked(connection); });
	}
}

void ConnectionsViewController::add_clicked()
{
}

void ConnectionsViewController::connect_clicked(XMLSettings connection)
{
	AppModel::instance()->connect_to_server(connection);
}

void ConnectionsViewController::edit_clicked(XMLSettings connection)
{
}

void ConnectionsViewController::remove_clicked(XMLSettings connection)
{
}
