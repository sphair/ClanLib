
#include "precomp.h"
#include "client.h"
#include "client_gui.h"
#include "login_events.h"
#include "login_view.h"
#include "lobby_events.h"
#include "lobby_view.h"
#include "lobby_model.h"
#include "game_events.h"
#include "game_view.h"
#include "game_model.h"
#include "../Lib/map_area.h"
#include "../Lib/net_events_lobby.h"
#include "../Lib/net_events_game.h"
#include "../Lib/net_events_login.h"

Client::Client()
: state(not_logged_in), game_view(0), lobby_view(0)
{
	slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
	slots.connect(network_client.sig_connected(), this, &Client::on_connected);
	slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

	gui.reset(new ClientGUI());

	game_events.reset(new GameEvents(this));
	lobby_events.reset(new LobbyEvents(this));
	login_events.reset(new LoginEvents(this));
}

Client::~Client()
{
}

GUIManager *Client::get_gui()
{
	return gui->get_gui();
}

// TODO: These network sending functions wasnt the best ideas
void Client::request_create_game()
{
	network_client.send_event(NetGameEvent(CTS_LOBBY_CREATE_GAME));
}

void Client::request_join_game(int game_id)
{
	network_client.send_event(NetGameEvent(CTS_LOBBY_JOIN_GAME, game_id));
}

void Client::request_start_game()
{
	network_client.send_event(NetGameEvent(CTS_LOBBY_START_GAME));
}

void Client::request_leave_game()
{
	network_client.send_event(NetGameEvent(CTS_LOBBY_LEAVE_GAME));
}

void Client::request_attack_area(int map_area_from_id, int map_area_to_id)
{
	network_client.send_event(NetGameEvent(CTS_GAME_ATTACK_AREA, map_area_from_id, map_area_to_id));
}

void Client::request_end_turn()
{
	network_client.send_event(NetGameEvent(CTS_GAME_END_TURN));
}

void Client::exec()
{
	std::auto_ptr<LoginView> dlg_connect(new LoginView(this));
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
		catch (Exception e)
		{
			Console::write_line("Unable to connect to %1(%2): %3", dlg_connect->get_server(), dlg_connect->get_port(), e.message);
			dlg_connect->show();
		}
	}
	dlg_connect.reset();

	lobby_view = new LobbyView(this);
	lobby_model.reset(new LobbyModel());

	gui->get_gui()->exec();
}

void Client::on_connected()
{
	network_client.send_event(NetGameEvent(CTS_LOGIN, player_nick));
}

void Client::on_disconnected()
{
	// TODO: Handle disconnection more gracefully
	throw Exception("Disconnected from server");
}

void Client::on_event_received(const NetGameEvent &e) 
{
	log_event("network", "Server sent event: %1", e.to_string());

	bool handled_event = false;

	// TODO: Fix state (playing state is never set)
	switch(state)
	{
	case not_logged_in:
		handled_event |= login_events->get_dispatcher().dispatch(e);
		break;
	case in_lobby:
	case playing:
		handled_event |= lobby_events->get_dispatcher().dispatch(e);
//		break;
//	case playing:
		handled_event |= game_events->get_dispatcher().dispatch(e);
		break;
	}

	if(!handled_event)
		log_event("error", "Uncaught event: %1", e.to_string());
}

void Client::create_game()
{
	game_model.reset(new GameModel());

	delete game_view;
	game_view = new GameView(this);

	lobby_view->set_visible(false);
}

void Client::load_map(const std::string &map_name)
{
	std::string filename = string_format("Resources/Maps/%1/map.xml", map_name);
	game_model->get_map()->load(map_name, filename);
	game_view->load_map();
}