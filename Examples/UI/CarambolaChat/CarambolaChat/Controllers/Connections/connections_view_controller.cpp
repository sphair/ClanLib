
#include "precomp.h"
#include "connections_view_controller.h"
#include "Views/Connections/network_list_view.h"
#include "Views/Connections/network_list_item_view.h"
#include "Models/XMLSettings/xml_settings_list.h"
#include "Models/app_model.h"

using namespace clan;

ConnectionsViewController::ConnectionsViewController()
{
	view->style()->set("flex: auto");
	view->style()->set("flex-direction: column");
	view->style()->set("margin: 11px");

	auto title = std::make_shared<LabelView>();
	title->style()->set("font: bold 16px/20px 'Source Sans Pro'");
	title->set_text("Chat Networks");
	view->add_subview(title);

	networks = std::make_shared<NetworkListView>();
	view->add_subview(networks);

	slots.connect(AppModel::instance()->cb_irc_session_created, this, &ConnectionsViewController::on_irc_session_created);
	slots.connect(AppModel::instance()->cb_irc_session_destroyed, this, &ConnectionsViewController::on_irc_session_destroyed);

#if defined(XMPP_SUPPORT)
	slots.connect(AppModel::instance()->cb_xmpp_session_created, this, &ConnectionsViewController::on_xmpp_session_created);
	slots.connect(AppModel::instance()->cb_xmpp_session_destroyed, this, &ConnectionsViewController::on_xmpp_session_destroyed);
#endif

	XMLSettingsList connections = AppModel::instance()->settings.xml_settings.get_list("connections");
	int connection_count = connections.get_count();
	for (int i = 0; i < connection_count; ++i)
	{
		XMLSettings connection = connections.get(i);
		std::string name = connection.get_string("connectionname");

		auto item = networks->add_item(name);

		slots.connect(item->connect_button->sig_pointer_release(), [=](PointerEvent &) { connect_clicked(connection); });
		slots.connect(item->edit_button->sig_pointer_release(), [=](PointerEvent &) { edit_clicked(connection); });
		slots.connect(item->remove_button->sig_pointer_release(), [=](PointerEvent &) { remove_clicked(connection); });
	}
}

void ConnectionsViewController::add_clicked()
{
	view->present_modal("Add Connection", edit_connection->view);
}

void ConnectionsViewController::connect_clicked(XMLSettings connection)
{
	AppModel::instance()->connect_to_server(connection);
}

void ConnectionsViewController::edit_clicked(XMLSettings connection)
{
	view->present_modal("Edit Connection", edit_connection->view);
}

void ConnectionsViewController::remove_clicked(XMLSettings connection)
{
}

void ConnectionsViewController::on_irc_session_created(IRCSession *session)
{
	slots.connect(session->cb_connect_status_changed, [=](IRCSession::ConnectStatus status) { on_session_connect_status_changed(status, session); });
}

void ConnectionsViewController::on_irc_session_destroyed(IRCSession *session)
{
	auto item = networks->get_item(session->get_connection_name());
	if (item)
	{
		item->set_status_offline("Not connected");
	}
}

#if defined(XMPP_SUPPORT)
void ConnectionsViewController::on_xmpp_session_created(XMPPSession *session)
{
	slots.connect(session->cb_error_text, this, &ServerListView::on_xmpp_session_error_text, session);
	slots.connect(session->cb_roster_updated, this, &ServerListView::on_xmpp_session_roster_updated, session);
}

void ConnectionsViewController::on_xmpp_session_destroyed(XMPPSession *session)
{
}

void ConnectionsViewController::on_xmpp_session_error_text(const CL_String &text, XMPPSession *session)
{
	int count = friends_view->get_item_count(index_group_networks);
	for (int i = 0; i < count; i++)
	{
		ConnectionItem *item = dynamic_cast<ConnectionItem*>(friends_view->get_item(index_group_networks, i));
		if (item && item->get_name() == session->get_connection_name())
		{
			break;
		}
	}
}

void ConnectionsViewController::on_xmpp_session_roster_updated(XMPPSession *session)
{
	if (index_group_friends == -1)
		index_group_friends = friends_view->add_group("Friends");
	else
		friends_view->clear_group(index_group_friends);

	std::vector<XMPPRosterItem> roster = session->get_roster();
	for (size_t i = 0; i < roster.size(); i++)
	{
		CL_Image icon = icon_offline;
		switch (roster[i].show_type)
		{
		case show_offline: icon = icon_offline; break;
		case show_away: icon = icon_away; break;
		case show_chat: icon = icon_chat; break;
		case show_dnd: icon = icon_dnd; break;
		case show_extended_away: icon = icon_extended_away; break;
		}
		FriendsViewItem *item = new FriendsViewItem(icon, roster[i].name, roster[i].show_text);
		friends_view->add_item(index_group_friends, item);
	}
}
#endif

void ConnectionsViewController::on_session_connect_status_changed(IRCSession::ConnectStatus status, IRCSession *session)
{
	auto item = networks->get_item(session->get_connection_name());
	if (item)
	{
		switch (status)
		{
		case IRCSession::status_connected:
			item->set_status_online("Connected");
			break;
		case IRCSession::status_connecting:
			item->set_status_online("Connecting..");
			break;
		case IRCSession::status_disconnected:
			item->set_status_offline("Not connected");
			break;
		default:
			item->set_status_online("Unknown");
			break;
		}
	}
}
