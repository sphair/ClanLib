
#include "precomp.h"
#include "server.h"
#include "server_lobby_events.h"
#include "server_player.h"
#include "server_lobby_model.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_lobby_game_player_collection.h"
#include "server_game.h"

ServerLobbyEvents::ServerLobbyEvents(Server *server, ServerLobbyModel *lobby_model)
: server(server), lobby_model(lobby_model)
{
	lobby_events.func_event("lobby-create-game").set(this, &ServerLobbyEvents::on_event_create_game);
	lobby_events.func_event("lobby-change-game-settings").set(this, &ServerLobbyEvents::on_event_change_game_settings);
	lobby_events.func_event("lobby-get-available-games").set(this, &ServerLobbyEvents::on_event_get_available_games);
	lobby_events.func_event("lobby-join-game").set(this, &ServerLobbyEvents::on_event_join_game);
	lobby_events.func_event("lobby-leave-game").set(this, &ServerLobbyEvents::on_event_leave_game);
	lobby_events.func_event("lobby-start-game").set(this, &ServerLobbyEvents::on_event_start_game);
	lobby_events.func_event("lobby-add-message").set(this, &ServerLobbyEvents::on_event_add_lobby_message);
}

ServerLobbyEvents::~ServerLobbyEvents()
{
}

void ServerLobbyEvents::on_event_create_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
{
	lobby_model->games.create_lobby_game(player);
}

void ServerLobbyEvents::on_event_change_game_settings(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
{
	CL_String game_name = e.get_argument(0);
	CL_String map_name = e.get_argument(1);
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
		player->send_event(CL_NetGameEvent("lobby-error-message", "You do not own a game"));
	}
}

void ServerLobbyEvents::on_event_get_available_games(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
{
	lobby_model->games.send_available_lobby_games(player);
}

void ServerLobbyEvents::on_event_join_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
{
	int lobby_game_id = e.get_argument(0);
	ServerLobbyGame *lobby_game = lobby_model->games.get_lobby_game(lobby_game_id);
	if(lobby_game)
		lobby_game->get_player_collection()->add_player(player);
	else
		player->send_event(CL_NetGameEvent("lobby-error-message", "Game not found"));
}

void ServerLobbyEvents::on_event_leave_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
{
	if(player->current_game)
	{
		player->current_game->get_player_collection()->remove_player(player);
		lobby_model->games.remove_empty_games();
	}
	else
	{
		player->send_event(CL_NetGameEvent("lobby-error-message", "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_start_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player)
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
			player->send_event(CL_NetGameEvent("lobby-error-message", "Can not start a game without owning it"));
		}
	}
	else
	{
		player->send_event(CL_NetGameEvent("lobby-error-message", "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_add_lobby_message(const CL_NetGameEvent &e, ServerLobbyPlayer *lobby_player)
{
	CL_String message = e.get_argument(0);
	if(message.length() > 0)
		server->get_network_server()->send_event(CL_NetGameEvent("lobby-player-message", lobby_player->player->id, message));
}
