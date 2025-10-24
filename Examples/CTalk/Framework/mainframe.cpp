
#include "precomp.h"
#include "mainframe.h"
#include "view.h"
#include "Document/document.h"
#include "Document/document.h"
#include "Document/irc_session.h"
#include "Document/xml_settings_list.h"
#include "Views/Chat/chatview.h"
#include "Views/ServerList/server_list_view.h"
#include "resource.h"
#include <algorithm>

MainFrame::MainFrame(CL_GUIManager *gui, CL_ResourceManager *resources)
: CL_Window(gui, get_window_description()), resources(resources), document(0), workspace(0)
{
	CL_DisplayWindow dispwindow = get_display_window();
	dispwindow.set_large_icon(CL_PNGProvider::load("carambola-32.png"));
	dispwindow.set_small_icon(CL_PNGProvider::load("carambola-16.png"));

	slots.connect(dispwindow.sig_window_minimized(), this, &MainFrame::on_minimized);

	notification_icon.cb_lbuttondblclk.set(this, &MainFrame::on_notification_icon_activate);
	notification_icon.show(IDI_APPICON);

	workspace = new ViewWorkspace(this);
	workspace->cb_view_close.set(this, &MainFrame::on_view_close);

	func_resized().set(this, &MainFrame::on_resized);
	func_close().set(this, &MainFrame::on_close);
	func_activation_gained().set(this, &MainFrame::on_activation_gained);

	document.reset(new Document);

	on_resized();

	add_server_list_view();

	auto_connect_to_servers();
}

MainFrame::~MainFrame()
{
	delete workspace;
}

CL_GUITopLevelDescription MainFrame::get_window_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Carambola Chat");
	desc.set_allow_resize(true);
	desc.set_position(CL_Rect(-1, -1, 900, 680), false);
	return desc;
}

Document *MainFrame::get_document()
{
	return document.get();
}

int MainFrame::get_num_views()
{
	return views.size();
}

View *MainFrame::get_view(int index)
{
	return views[index];
}

View *MainFrame::get_active_view()
{
	return views[workspace->get_current_view_index()];
}

CL_Rect MainFrame::get_view_area()
{
	//return CL_Rect(8, 8+16, get_width()-8, get_height()-8);
	return CL_Rect(0, 22, get_width(), get_height());
}

CL_ResourceManager *MainFrame::get_resources()
{
	return resources;
}

void MainFrame::set_active_view(View *view)
{
	set_active_view(find_view_index(view));
}

void MainFrame::set_active_view(int index)
{
	workspace->show_view(index);
}

void MainFrame::add_server_list_view()
{
	View *view = new ServerListView(workspace, this);
	views.push_back(view);
	workspace->add_view(view);
	workspace->show_view(view);
}

void MainFrame::add_view(const IRCEntity &filter, IRCSession *session)
{
	View *view = new ChatView(session, filter, workspace, this);
	views.push_back(view);
	workspace->add_view(view);
	workspace->show_view(view);
}

void MainFrame::remove_view(View *view)
{
	workspace->remove_view(view);
	int index = find_view_index(view);
	delete views[index];
	views.erase(views.begin() + index);
}

int MainFrame::find_view_index(View *view)
{
	int size = views.size();
	for (int i=0; i<size; i++)
	{
		if (views[i] == view)
		{
			return i;
		}
	}
	throw CL_Exception("View not found in collection");
}

ChatView *MainFrame::find_view(const IRCEntity &filter)
{
	int num_views = get_num_views();
	for (int i=0; i<num_views; i++)
	{
		View *view = get_view(i);
		ChatView *chatview = dynamic_cast<ChatView *>(view);
		if (chatview)
		{
			if (chatview->get_filter() == filter)
				return chatview;
		}
	}
	return 0;
}

void MainFrame::flag_activity(View *view)
{
	workspace->set_view_color(view, CL_Colorf::red);

	flash_window(view);
}

void MainFrame::flash_window(View * view)
{
	CL_DisplayWindow dispwindow = get_display_window();
	if(dispwindow.has_focus() == false || get_active_view() != view)
	{
		#ifdef WIN32
		HWND hwnd = dispwindow.get_hwnd();
		FlashWindow(hwnd, TRUE);
		#endif

		notification_icon.start_flash();
	}
}

void MainFrame::auto_connect_to_servers()
{
	XMLSettingsList connections = get_document()->settings.get_list("connections");
	int connection_count = connections.get_count();
	for(int i = 0; i < connection_count; ++i)
	{
		XMLSettings connection = connections.get(i);

		if(connection.get_bool("autoconnect"))
		{
			connect_to_server(connection);
		}
	}
}

void MainFrame::connect_to_server(XMLSettings &connection)
{
	CL_String connection_name = connection.get_string("connectionname");
	CL_String nick = connection.get_string("nick", "CTalk2009");
	CL_String username = connection.get_string("username", "carambola");
	CL_String name = connection.get_string("name", "Anonymous ClanLib Carambola User");
	CL_String server_name = connection.get_string("server");

	std::vector<CL_String> perform;
	XMLSettingsList performlist = connection.get_list("performlist");
	int perform_count = performlist.get_count();
	for (int i=0; i<perform_count; i++)
	{
		XMLSettings item = performlist.get(i);
		perform.push_back(item.get_string("command"));
	}

	IRCSession *session = document->create_session(connection_name);
	slots.connect(session->cb_joined, this, &MainFrame::on_connection_join, session);
	slots.connect(session->cb_private_text, this, &MainFrame::on_connection_private_text, session);
	session->set_perform_list(perform);
	session->connect(server_name, "6667", nick, username, name);
	add_view(IRCEntity(), session);
}

void MainFrame::on_close()
{
	exit_with_code(0);
}

void MainFrame::on_view_close(View *view)
{
	std::vector<View *>::iterator it = std::find(views.begin(), views.end(), view);
	if (it != views.end())
	{
		views.erase(it);
		workspace->remove_view(view);
		delete view;
	}
}

void MainFrame::on_resized()
{
	CL_Rect client_size = get_size();
	workspace->set_geometry(client_size);
}

void MainFrame::on_minimized()
{
	set_visible(false);
}

void MainFrame::on_activation_gained()
{
	notification_icon.end_flash();
}

void MainFrame::on_connection_join(const IRCChannel &channel, IRCSession *session)
{
	if (find_view(channel) == 0)
		add_view(channel, session);
}

void MainFrame::on_connection_private_text(const IRCNick &nick, const IRCText &text, IRCSession *session)
{
	if (find_view(nick) == 0 && !text.is_ctcp())
	{
		add_view(nick, session);
		ChatView *view = find_view(nick);
		view->add_private_text(nick, text);
	}
}

void MainFrame::on_notification_icon_activate()
{
	set_visible(true, true);
}

void MainFrame::open_conversation(const IRCNick &nick, IRCSession *session)
{
	View *view = find_view(nick);
	if (view)
	{
		workspace->show_view(view);
	}
	else
	{
		add_view(nick, session);
	}
}
