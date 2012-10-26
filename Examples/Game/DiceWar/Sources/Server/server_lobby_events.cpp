
#include "precomp.h"
#include "server.h"
#include "server_lobby_events.h"
#include "server_player.h"
#include "server_lobby_model.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_lobby_game_player_collection.h"
#include "server_game.h"
#include "../Lib/net_events_lobby.h"

ServerLobbyEvents::ServerLobbyEvents(Server *server, ServerLobbyModel *lobby_model)
: server(server), lobby_model(lobby_model)
{
	lobby_events.func_event(CTS_LOBBY_CREATE_GAME).set(this, &ServerLobbyEvents::on_event_create_game);
	lobby_events.func_event(CTS_LOBBY_CHANGE_GAME_SETTINGS).set(this, &ServerLobbyEvents::on_event_change_game_settings);
	lobby_events.func_event(CTS_LOBBY_GET_AVAILABLE_GAMES).set(this, &ServerLobbyEvents::on_event_get_available_games);
	lobby_events.func_event(CTS_LOBBY_JOIN_GAME).set(this, &ServerLobbyEvents::on_event_join_game);
	lobby_events.func_event(CTS_LOBBY_LEAVE_GAME).set(this, &ServerLobbyEvents::on_event_leave_game);
	lobby_events.func_event(CTS_LOBBY_START_GAME).set(this, &ServerLobbyEvents::on_event_start_game);
	lobby_events.func_event(CTS_LOBBY_ADD_MESSAGE).set(this, &ServerLobbyEvents::on_event_add_lobby_message);
}

ServerLobbyEvents::~ServerLobbyEvents()
{
}

void ServerLobbyEvents::on_event_create_game(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	lobby_model->games.create_lobby_game(player);
}

void ServerLobbyEvents::on_event_change_game_settings(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	std::string game_name = e.get_argument(0);
	std::string map_name = e.get_argument(1);
	int max_players = e.get_argument(2);

	if(player->current_game)
	{
		ServerLobbyGame *lobby_game = player->current_game;
		lobby_game->set_map_name(map_name);
		lobby_game->set_max_players(max_players);
		lobby_game->set_name(game_name);

		lobby_game->send_game_info();
	}
	else
	{
		player->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "You do not own a game"));
	}
}

void ServerLobbyEvents::on_event_get_available_games(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	lobby_model->games.send_available_lobby_games(player);
}

void ServerLobbyEvents::on_event_join_game(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	int lobby_game_id = e.get_argument(0);
	ServerLobbyGame *lobby_game = lobby_model->games.get_lobby_game(lobby_game_id);
	if(lobby_game)
		lobby_game->get_player_collection()->add_player(player);
	else
		player->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Game not found"));
}

void ServerLobbyEvents::on_event_leave_game(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	if(player->current_game)
	{
		player->current_game->get_player_collection()->remove_player(player);
		lobby_model->games.remove_empty_games();
	}
	else
	{
		player->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_start_game(const NetGameEvent &e, ServerLobbyPlayer *player)
{
	if(player->current_game)
	{
		if(player->current_game->get_player_collection()->get_owner() == player)
		{
			ServerGame *game = lobby_model->games.create_game(player->current_game);
			if(game->start())
				player->current_game->set_state(ServerLobbyGame::playing);
		}
		else
		{
			player->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Can not start a game without owning it"));
		}
	}
	else
	{
		player->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_add_lobby_message(const NetGameEvent &e, ServerLobbyPlayer *lobby_player)
{
	std::string message = e.get_argument(0);
	if(message.length() > 0)
		server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_PLAYER_MESSAGE, lobby_player->player->id, message));
}
