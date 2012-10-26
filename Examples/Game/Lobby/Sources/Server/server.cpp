
#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_game.h"
#include "my_server_lobby_game.h"
#include "my_server_lobby_player.h"
#include "../Common/net_events_login.h"

Server::Server()
: next_player_id(1), next_game_id(1), lobby(&network_server)
{
	slots.connect(network_server.sig_client_connected(), this, &Server::on_client_connected);
	slots.connect(network_server.sig_client_disconnected(), this, &Server::on_client_disconnected);
	slots.connect(network_server.sig_event_received(), this, &Server::on_event_received);

	login_events.func_event(CTS_LOGIN).set(this, &Server::on_event_login);

	lobby.func_create_lobby_game.set(this, &Server::on_create_lobby_game);
	lobby.func_create_lobby_player.set(this, &Server::on_create_lobby_player);
//	lobby.on_game_state_changed.set(this, &Server::on_lobby_game_state_changed);
}

Server::~Server()
{
}

void Server::exec(CL_Event &stop_event)
{
	try
	{
		network_server.start("4568");

		while (true)
		{
			CL_KeepAlive::process(10);

			if (stop_event.wait(0)) 
				break;
		}

		network_server.stop();
	}
	catch (CL_Exception e)
	{
		cl_log_event("Exception", e.get_message_and_stack_trace());
	}
}

void Server::on_client_connected(CL_NetGameConnection *connection)
{
	cl_log_event("system", "Client connected");

	// create player -> this also stored it on the network connection object
	ServerPlayer *player = new ServerPlayer(connection);
}

void Server::on_client_disconnected(CL_NetGameConnection *connection)
{
	cl_log_event("system", "Client disconnected");

	ServerPlayer *player = ServerPlayer::get_player(connection);

	if(player)
	{
		lobby.remove_player(player->get_id());
		delete player;
	}
}

void Server::login(ServerPlayer *player, CL_String player_name)
{
	cl_log_event("system", "Client logged in");

	if (player->is_logged_in() == false)
	{
		player->login(next_player_id++, player_name);
		player->send_event(CL_NetGameEvent(STC_LOGIN_SUCCESSFUL, player->get_id()));

		lobby.add_player(player->get_id(), player->get_connection());
	}
	else
	{
		player->send_event(CL_NetGameEvent(STC_LOGIN_FAILED, "Already logged in"));
	}
}

void Server::on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &e)
{
	cl_log_event("system", "Client sent event: %1", e.to_string());

	ServerPlayer *player = ServerPlayer::get_player(connection);

	bool handled_event = false;
	if (player->is_logged_in() == false)
	{
		handled_event = login_events.dispatch(e, player);
	}
	else
	{
		handled_event = lobby.dispatch_event(e, player->get_id());
	}

	if (!handled_event)
		cl_log_event("system", "Unhandled event: %1", e.to_string());
}

void Server::on_event_login(const CL_NetGameEvent &e, ServerPlayer *player)
{
	CL_String player_name = e.get_argument(0);

	login(player, player_name);
}

ServerLobbyGameInformation *Server::on_create_lobby_game(CL_NetGameConnection *owner_player_connection)
{
	ServerPlayer *player = ServerPlayer::get_player(owner_player_connection);
	MyServerLobbyGameInformation *lobby_game = new MyServerLobbyGameInformation();
	lobby_game->set_name(cl_format("%1s game", player->get_name()));
	return lobby_game;
}

ServerLobbyPlayerInformation *Server::on_create_lobby_player(CL_NetGameConnection *player_connection)
{
	ServerPlayer *player = ServerPlayer::get_player(player_connection);
	MyServerLobbyPlayerInformation *lobby_player = new MyServerLobbyPlayerInformation();
	lobby_player->set_name(player->get_name());
	return lobby_player;
}
/*
bool Server::create_game(const std:vector<CL_NetGameConnection *> &players, ServerLobbyGameInformation *lobby_game_information)
{
	MyServerLobbyGameInformation *game_information = (MyServerLobbyGameInformation *)lobby_game_information;

	// Move this to caller
	ServerLobbyGame *lobby_game = player->get_current_game();
	lobby_game->set_state(ServerLobbyGame::playing);

	// Guard against exceptions between creation and putting it into collection
	std::auto_ptr<ServerGame> game(new ServerGame(server, next_game_id++));

	game->set_map_name(game_information->get_map_name());

	std:vector<CL_NetGameConnection *>::const_iterator it;
	for(it = players.begin(); it != players.end(); ++it)
		game->add_player(ServerPlayer::get_player(*it));

	running_games.push_back(game.get());

	game.release();		

	return true;
}
*/