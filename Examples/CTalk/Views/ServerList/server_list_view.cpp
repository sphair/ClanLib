
#include "precomp.h"
#include "server_list_view.h"
#include "server_general_page.h"
#include "Framework/mainframe.h"
#include "Framework/preferences_viewer.h"
#include "Document/document.h"
#include "Document/xml_settings_list.h"

ServerListView::ServerListView(CL_GUIComponent *parent, MainFrame *mainframe)
: View(parent, mainframe, "Connections")
{
	func_resized().set(this, &ServerListView::on_resized);

	server_list = new CL_ListView(this);
	server_list->get_header()->append(server_list->get_header()->create_column("network", "IRC Network"));
	server_list->get_header()->append(server_list->get_header()->create_column("nick", "Nick"));
	server_list->get_header()->append(server_list->get_header()->create_column("status", "Status"));
	server_list->get_header()->append(server_list->get_header()->create_column("comments", "Comments"));
	server_list->set_select_whole_row(true);
	server_list->func_selection_changed().set(this, &ServerListView::on_server_list_selection_changed);

	button_add = new CL_PushButton(this);
	button_add->set_text("Add");
	button_remove = new CL_PushButton(this);
	button_remove->set_text("Remove");
	button_connect = new CL_PushButton(this);
	button_connect->set_text("Connect");
	button_edit = new CL_PushButton(this);
	button_edit->set_text("Edit");

	button_add->func_clicked().set(this, &ServerListView::on_button_add_clicked);
	button_remove->func_clicked().set(this, &ServerListView::on_button_remove_clicked);
	button_connect->func_clicked().set(this, &ServerListView::on_button_connect_clicked);
	button_edit->func_clicked().set(this, &ServerListView::on_button_edit_clicked);

	button_remove->set_enabled(false);
	button_connect->set_enabled(false);
	button_edit->set_enabled(false);

	slots.connect(get_mainframe()->get_document()->cb_session_created, this, &ServerListView::on_session_created);
	slots.connect(get_mainframe()->get_document()->cb_session_destroyed, this, &ServerListView::on_session_destroyed);

	XMLSettingsList connections = get_mainframe()->get_document()->settings.get_list("connections");
	int connection_count = connections.get_count();
	for(int i = 0; i < connection_count; ++i)
	{
		XMLSettings connection = connections.get(i);
	
		CL_ListViewItem item = server_list->create_item();
		item.set_column_text("network", connection.get_string("connectionname"));
		item.set_column_text("nick", connection.get_string("nick"));
		item.set_column_text("status", "Not connected");
		item.set_id(i);
		server_list->get_document_item().append_child(item);
	}

	on_resized();
}

ServerListView::~ServerListView()
{
}

void ServerListView::on_resized()
{
	int button_height = 21;
	int spacing = 7;

	server_list->set_geometry(CL_Rect(0, 0, get_width(), get_height()-button_height-spacing));
	button_connect->set_geometry(get_button_geometry(0,0));
	button_add->set_geometry(get_button_geometry(1,1));
	button_remove->set_geometry(get_button_geometry(2,1));
	button_edit->set_geometry(get_button_geometry(3,1));
}

CL_Rect ServerListView::get_button_geometry(int index, int separators)
{
	int button_width = 75;
	int button_height = 21;
	int spacing = 7;
	int x = (button_width+spacing)*index;
	int y = get_height()-button_height;
	return CL_Rect(CL_Point(x,y), CL_Size(button_width, button_height));
	//return CL_Rect(4+70*index+separators*12, get_height()-26, 4+70*index+64+separators*12, get_height()-4);
}

void ServerListView::on_server_list_selection_changed(CL_ListViewSelection selection)
{
	bool contains_items = !selection.get_first().is_null();
	button_remove->set_enabled(contains_items);
	button_connect->set_enabled(contains_items);
	button_edit->set_enabled(contains_items);

	// Bug in clanGUI not causing them to automatically repaint by above action
	button_remove->request_repaint();
	button_connect->request_repaint();
	button_edit->request_repaint();
}

void ServerListView::on_button_add_clicked()
{

}

void ServerListView::on_button_remove_clicked()
{

}

void ServerListView::on_button_connect_clicked()
{
	CL_ListViewItem item = server_list->get_selected_item();
	if (!item.is_null())
	{
		XMLSettingsList connections = get_mainframe()->get_document()->settings.get_list("connections");
		XMLSettings connection = connections.get(item.get_id());
		get_mainframe()->connect_to_server(connection);
		item.set_column_text("status", "Connected");
	}
}

void ServerListView::on_button_edit_clicked()
{
	PreferencesViewer viewer(this);
	viewer.add_page(new ServerGeneralPage(&viewer));
	// viewer.add_page(new PerformPage(&viewer));
	viewer.set_visible(true, true);
	viewer.exec();
}

void ServerListView::on_session_created(IRCSession *session)
{
	slots.connect(session->cb_connect_status_changed, this, &ServerListView::on_session_connect_status_changed, session);
}

void ServerListView::on_session_destroyed(IRCSession *session)
{
}

void ServerListView::on_session_connect_status_changed(IRCSession::ConnectStatus status, IRCSession *session)
{
	CL_ListViewItem document = server_list->get_document_item();

	CL_ListViewItem item = document.get_first_child();
	while (item.is_item())
	{
		CL_String connection_name = item.get_column(cl_text("network")).get_text();

		if(connection_name == session->get_connection_name())
		{
			if(status == IRCSession::status_connecting)
				item.set_column_text("status", "Connecting");
			else if(status == IRCSession::status_connected)
				item.set_column_text("status", "Connected");
			else if(status == IRCSession::status_disconnected)
				item.set_column_text("status", "Disconnected");
			else
				item.set_column_text("status", "Unknown");

			break;
		}
		item = item.get_next_sibling();
	}
}

XMLSettings ServerListView::get_settings()
{
	return get_mainframe()->get_document()->settings.get_section("server-list");
}
