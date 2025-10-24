
#include "precomp.h"
#include "mainframe.h"
#include "document.h"
#include "view.h"
#include "ircconnection.h"
#include "chatview.h"
#include "server_list_view.h"

/////////////////////////////////////////////////////////////////////////////
// MainFrame construction:

MainFrame::MainFrame(CL_GUIManager *gui, CL_ResourceManager *resources)
: CL_Window(CL_Rect(-1, -1, 740, 580), gui, get_window_description()), resources(resources), document(0), tab(0)
{
	tab = new CL_Tab(this);
	tab->set_geometry(CL_Rect(CL_Point(0,0), get_size()));

	func_resized().set(this, &MainFrame::on_resized);
	func_close().set(this, &MainFrame::on_close);

	document.reset(new Document);

	add_server_list_view();

	timer = create_timer();
	timer.func_expired().set(this, &MainFrame::on_timer_expired);
	timer.start(250);
}

MainFrame::~MainFrame()
{
}

CL_GUITopLevelDescription MainFrame::get_window_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("CTalk IRC Client");
	desc.set_allow_resize(true);
	return desc;
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame attributes:

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
	return views[tab->get_current_page_index()];
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

/////////////////////////////////////////////////////////////////////////////
// MainFrame operations:

void MainFrame::set_active_view(View *view)
{
	set_active_view(find_view_index(view));
}

void MainFrame::set_active_view(int index)
{
	tab->show_page(index);
}

void MainFrame::add_server_list_view()
{
	CL_TabPage *page = tab->add_page("");
	View *view = new ServerListView(page, this);
	views.push_back(view);
	tab->set_label(views.size()-1, view->get_title());
	tab->show_page(views.size()-1);
	view->set_geometry(CL_Rect(CL_Point(0,0), page->get_size()));
}

void MainFrame::add_view(const CL_String &channel, IRCConnection *connection)
{
	CL_TabPage *page = tab->add_page("");
	View *view = new ChatView(connection, channel, page, this);
	views.push_back(view);
	tab->set_label(views.size()-1, view->get_title());
	tab->show_page(views.size()-1);
	view->set_geometry(CL_Rect(CL_Point(0,0), page->get_size()));
}

void MainFrame::remove_view(View *view)
{
	int index = find_view_index(view);
	delete views[index];
	tab->remove_page(index);
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

void MainFrame::connect_to_server(const CL_String &server_name, const CL_String &nick, const CL_String &username, const CL_String &full_name, const CL_String &password)
{
	IRCConnection *connection = new IRCConnection(server_name);
	get_document()->connections.push_back(connection);
	add_view(CL_String(), connection);
	connection->send_nick(nick);
	connection->send_user(username, "localhost", server_name, full_name);
	connection->set_nick(nick);
	slots.connect(connection->sig_join(), this, &MainFrame::on_connection_join, connection);
	if (!password.empty())
		connection->send_pass(password);
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame implementation:

void MainFrame::on_close()
{
	exit_with_code(0);
}

void MainFrame::on_resized()
{
	tab->set_geometry(CL_Rect(CL_Point(0,0), get_size()));
	int index = tab->get_current_page_index();
	views[index]->set_geometry(tab->get_page(index)->get_size());
}

void MainFrame::on_connection_join(const CL_String &nick, const CL_String &channel, IRCConnection *connection)
{
	// Check if we got a view for this channel yet:
	int num_views = get_num_views();
	for (int i=0; i<num_views; i++)
	{
		View *view = get_view(i);
		ChatView *chatview = dynamic_cast<ChatView *>(view);
		if (chatview)
		{
			if (chatview->get_filter() == channel) return;
		}
	}

	// Ok no view, create new one:
	add_view(channel, connection);
}

void MainFrame::on_timer_expired()
{
	for (std::list<IRCConnection *>::iterator it = document->connections.begin(); it != document->connections.end(); ++it)
	{
		(*it)->process_data();
	}
}
