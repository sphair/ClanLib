
#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_login_events.h"
#include "server_lobby_events.h"
#include "server_lobby_model.h"
#include "server_game_events.h"
#include "server_game_player.h"
#include "../Lib/net_events_login.h"
//#include "server_test.h"

Server::Server()
: next_player_id(1)
{
	slots.connect(network_server.sig_client_connected(), this, &Server::on_client_connected);
	slots.connect(network_server.sig_client_disconnected(), this, &Server::on_client_disconnected);
	slots.connect(network_server.sig_event_received(), this, &Server::on_event_received);

	login_events.reset(new ServerLoginEvents(this));

	lobby_model.reset(new ServerLobbyModel(this));
	lobby_events.reset(new ServerLobbyEvents(this, lobby_model.get()));

	game_events.reset(new ServerGameEvents(this));

//	ServerTest test(this);
}

Server::~Server()
{
}

void Server::exec(Event &stop_event)
{
	try
	{
		network_server.start("4558");

		while (true)
		{
			KeepAlive::process(10);

			if (stop_event.wait(0)) 
				break;
		}

		network_server.stop();
	}
	catch (Exception e)
	{
		cl_log_event("Exception", e.get_message_and_stack_trace());
	}
}

void Server::on_client_connected(NetGameConnection *connection)
{
	cl_log_event("system", "Client connected");

	// create player -> this also attaches it to the network connection
	ServerPlayer *player = new ServerPlayer(connection);
}

void Server::on_client_disconnected(NetGameConnection *connection)
{
	cl_log_event("system", "Client disconnected");

	ServerPlayer *player = ServerPlayer::get_player(connection);

	if(player)
	{
		lobby_model->players.remove_player(player);
		lobby_model->games.remove_empty_games();

		delete player;
	}
}

void Server::login(ServerPlayer *player, std::string player_name)
{
	cl_log_event("system", "Client logged in");

	if (player->id == 0) // player not logged in
	{
		player->id = next_player_id++;
		player->name = player_name;
		player->send_event(NetGameEvent(STC_LOGIN_SUCCESSFUL, player->id));

		lobby_model->players.create_player(player, player_name);
	}
	else
	{
		player->send_event(NetGameEvent(STC_LOGIN_FAILED, "Already logged in"));
	}
}

ServerPlayer *Server::create_ai_player(const std::string &player_name)
{
	ServerPlayer *server_player = new ServerPlayer(0);
	server_player->id = next_player_id++;
	server_player->name = player_name;
	return server_player;
}

void Server::on_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	cl_log_event("system", "Client sent event: %1", e.to_string());

	ServerPlayer *player = ServerPlayer::get_player(connection);

	handle_event(player, e);
}

// TODO: This method should not be public, but it needs to currently to make it possible 
// for AI player to directly send an event to server
void Server::handle_event(ServerPlayer *player, const NetGameEvent &e)
{
	bool handled_event = false;
	if (player->id == 0)
	{
		handled_event = login_events->get_dispatcher().dispatch(e, player);
	}
	else
	{
		if (player->lobby_player)
			handled_event = lobby_events->get_dispatcher().dispatch(e, player->lobby_player);

		if (player->game_player)
			if(player->game_player->current_game)
				handled_event |= game_events->get_dispatcher().dispatch(e, player->game_player->current_game, player->game_player);
	}

	if (!handled_event)
		cl_log_event("system", "Unhandled event: %1", e.to_string());
}

