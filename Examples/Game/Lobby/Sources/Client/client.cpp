
#include "precomp.h"
#include "client.h"
#include "login_events.h"
#include "login_view.h"
#include "../Common/net_events_login.h"

Client::Client()
: state(not_logged_in)
{
	initialize_display();
	initialize_gui();
	initialize_network();
}

Client::~Client()
{
	delete login_events;
}

void Client::initialize_display()
{
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib - Lobby Client");
	desc.set_size(clan::Size(1024, 768), true);
	display_window = clan::DisplayWindow(desc);

	slots.connect(display_window.sig_window_close(), this, &Client::on_window_close);

	image_desktop = clan::Image(display_window.get_gc(), "Resources/GUITheme/Aquaburst 1024x768.jpg");
}

void Client::initialize_gui()
{
	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("Not default themes found");

	gui = clan::GUIManager(display_window);
	clan::GUIWindowManagerTexture &manager = (clan::GUIWindowManagerTexture)gui.get_window_manager();
	manager.func_repaint().set(this, &Client::on_gui_paint);

	gui.add_theme(theme + "/theme.css");
	gui.add_resources(theme + "/resources.xml");
	gui.add_theme("Resources/GUITheme/theme.css");
	gui.add_resources("Resources/GUITheme/resources.xml");
}

void Client::on_gui_paint()
{
	clan::GUIWindowManagerTexture &manager = (clan::GUIWindowManagerTexture)gui.get_window_manager();

	clan::Canvas canvas(display_window);
	image_desktop.draw(canvas, canvas.get_size());

	manager.draw_windows(canvas);

	display_window.flip();
}

void Client::initialize_network()
{
	slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
	slots.connect(network_client.sig_connected(), this, &Client::on_connected);
	slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

	login_events = new LoginEvents(this);
}

void Client::on_window_close()
{
	gui.exit_with_code(0);
}

void Client::exec()
{
	std::auto_ptr<LoginView> dlg_connect(new LoginView(&gui));
	while (true)
	{
		if (dlg_connect->exec() == 0)
			return;

		try
		{
			network_client.connect(dlg_connect->get_server(), dlg_connect->get_port());
			player_nick = dlg_connect->get_player_nick();
			break;
		}
		catch (clan::Exception e)
		{
			clan::Console::write_line("Unable to connect to %1(%2): %3", dlg_connect->get_server(), dlg_connect->get_port(), e.message);
			dlg_connect->show();
		}
	}
	dlg_connect.reset();

	gui.exec();
}

void Client::on_connected()
{
	network_client.send_event(clan::NetGameEvent(CTS_LOGIN, player_nick));
}

void Client::on_disconnected()
{
	// TODO: Handle disconnection more gracefully
	throw clan::Exception("Disconnected from server");
}

void Client::on_event_received(const clan::NetGameEvent &e) 
{
	clan::log_event("network", "Server sent event: %1", e.to_string());

	bool handled_event = false;

	switch(state)
	{
	case not_logged_in:
		handled_event |= login_events->get_dispatcher().dispatch(e);
		break;
/*	case in_lobby:
		handled_event |= lobby_events->get_dispatcher().dispatch(e);
		break;
	case playing:
		handled_event |= game_events->get_dispatcher().dispatch(e);
		break;
*/	}

	if(!handled_event)
		clan::log_event("error", "Uncaught event: %1", e.to_string());
}
