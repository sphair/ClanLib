
#include "precomp.h"
#include "mainframe.h"
#include "document.h"
#include "view.h"
#include "ircconnection.h"
#include "chatview.h"
#include "dlg_connect.h"

/////////////////////////////////////////////////////////////////////////////
// MainFrame construction:

MainFrame::MainFrame(CL_Component *parent, CL_ResourceManager *resources) :
	CL_Frame(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()), parent),
	resources(resources),
	document(0),
	active_view(-1)
{
	document = new Document;
	slots.connect(sig_paint(), this, &MainFrame::on_paint);

	DlgConnect dlg(this);
	dlg.run();

	IRCConnection *connection = new IRCConnection(dlg.get_server());
	document->connections.push_back(connection);
	add_view(new ChatView(connection, std::string(), this));
	connection->send_nick(dlg.get_nick());
	connection->send_user(dlg.get_username(), "localhost", dlg.get_server(), dlg.get_fullname());
	connection->set_nick(dlg.get_nick());
	slots.connect(connection->sig_join(), this, &MainFrame::on_connection_join, connection);
	if (!dlg.get_password().empty()) connection->send_pass(dlg.get_password());
}

MainFrame::~MainFrame()
{
	delete document;
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame attributes:

Document *MainFrame::get_document()
{
	return document;
}

int MainFrame::get_num_views()
{
	return views.size();
}

View *MainFrame::get_view(int index)
{
	return views[index].second;
}

View *MainFrame::get_active_view()
{
	if (active_view == -1) return 0;
	return views[active_view].second;
}

CL_Rect MainFrame::get_view_area()
{
	return CL_Rect(8, 8+16, get_width()-8, get_height()-8);
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
	if (active_view != -1) views[active_view].second->show(false);
	active_view = index;

	views[active_view].second->set_position(get_view_area());
	views[active_view].second->show(true);
}

void MainFrame::add_view(View *view)
{
	CL_Button *button = new CL_Button(view->get_title(), this);
	slots.connect(button->sig_clicked(), this, &MainFrame::on_view_button_clicked, view);
	views.push_back(std::pair<CL_Button *, View *>(button, view));
	arrange_buttons();
	set_active_view(views.size()-1);
}

void MainFrame::remove_view(View *view)
{
	int index = find_view_index(view);
	delete views[index].first;
	views.erase(views.begin() + index);
	arrange_buttons();
}

int MainFrame::find_view_index(View *view)
{
	int size = views.size();
	for (int i=0; i<size; i++)
	{
		if (views[i].second == view)
		{
			return i;
		}
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame implementation:

void MainFrame::arrange_buttons()
{
	int pos = 0;
	int size = views.size();
	for (int i=0; i<size; i++)
	{
		views[i].first->set_position(pos, 0);
		pos += views[i].first->get_width();
	}
}

void MainFrame::on_paint()
{
	CL_Display::clear(CL_Color(240, 242, 244));

/*
	if (active_view == -1)
	{
		CL_Display::clear(CL_Color(80, 128, 80));
	}
	else
	{
		CL_Gradient gradient(
			CL_Color::aliceblue, CL_Color::lightskyblue, CL_Color::aqua, CL_Color::deepskyblue);
		CL_Display::fill_rect(CL_Rect(0, 0, get_width(), 16), gradient);
	}
*/
}

void MainFrame::on_view_button_clicked(View *view)
{
	set_active_view(view);
	view->set_focus();
}

void MainFrame::on_connection_join(const std::string &nick, const std::string &channel, IRCConnection *connection)
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
	add_view(new ChatView(connection, channel, this));
}
