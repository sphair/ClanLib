
#include "precomp.h"
#include "server_list_view.h"
#include "ircconnection.h"
#include "mainframe.h"
#include "document.h"

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

	CL_ListViewItem item = server_list->create_item();
	item.set_column_text("network", "Quakenet");
	item.set_column_text("nick", "CL_CTalk");
	item.set_column_text("status", "Not connected");
	item.set_id(0);
	server_list->get_document_item().append_child(item);

	item = server_list->create_item();
	item.set_column_text("network", "Freenode");
	item.set_column_text("nick", "CTalk09");
	item.set_column_text("status", "Not connected");
	item.set_id(1);
	server_list->get_document_item().append_child(item);

	on_resized();
}

ServerListView::~ServerListView()
{
}

void ServerListView::on_resized()
{
	server_list->set_geometry(CL_Rect(0, 0, get_width(), get_height()-28));
	button_connect->set_geometry(get_button_geometry(0,0));
	button_add->set_geometry(get_button_geometry(1,1));
	button_remove->set_geometry(get_button_geometry(2,1));
	button_edit->set_geometry(get_button_geometry(3,1));
}

CL_Rect ServerListView::get_button_geometry(int index, int separators)
{
	return CL_Rect(70*index+separators*10, get_height()-24, 70*index+68+separators*10, get_height());
}

void ServerListView::on_server_list_selection_changed(CL_ListViewSelection selection)
{
	bool contains_items = !selection.get_first().is_null();
	button_remove->set_enabled(contains_items);
	button_connect->set_enabled(contains_items);
	button_edit->set_enabled(contains_items);

	// Bug in clanGUI not causing them to automatically repaint by above action
	button_remove->invalidate_rect();
	button_connect->invalidate_rect();
	button_edit->invalidate_rect();
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
		int id = item.get_id();
		item.set_column_text("status", "Connected");
		if (id == 0)
			get_mainframe()->connect_to_server("irc.quakenet.org", "CL_CTalk", "ctalk", "Anonymous ClanLib CTalk User");
		else if (id == 1)
			get_mainframe()->connect_to_server("irc.freenode.net", "CTalk09", "ctalk", "Anonymous ClanLib CTalk User");
	}
}

void ServerListView::on_button_edit_clicked()
{

}
