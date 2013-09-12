
#include "precomp.h"
#include "server_lobby_events.h"
#include "server_lobby.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_lobby_game_collection.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_net_events.h"

ServerLobbyEvents::ServerLobbyEvents(clan::NetGameServer *server, ServerLobby *lobby, ServerLobbyGameCollection *games)
: server(server), lobby(lobby), games(games)
{
	lobby_events.func_event(CTS_LOBBY_CREATE_GAME).set(this, &ServerLobbyEvents::on_event_create_game);
	lobby_events.func_event(CTS_LOBBY_CHANGE_GAME_SETTINGS).set(this, &ServerLobbyEvents::on_event_change_game_settings);
	lobby_events.func_event(CTS_LOBBY_GET_AVAILABLE_GAMES).set(this, &ServerLobbyEvents::on_event_get_available_games);
	lobby_events.func_event(CTS_LOBBY_JOIN_GAME).set(this, &ServerLobbyEvents::on_event_join_game);
	lobby_events.func_event(CTS_LOBBY_LEAVE_GAME).set(this, &ServerLobbyEvents::on_event_leave_game);
	lobby_events.func_event(CTS_LOBBY_START_GAME).set(this, &ServerLobbyEvents::on_event_start_game);
	lobby_events.func_event(CTS_LOBBY_ADD_MESSAGE).set(this, &ServerLobbyEvents::on_event_add_lobby_message);
}

void ServerLobbyEvents::on_event_create_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	if (player->get_current_game())
	{
		player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Cannot create game when already joined a game"));
	}
	else
	{
		ServerLobbyGame *lobby_game = games->create_lobby_game(player, lobby->func_create_lobby_game);

		player->send_event(clan::NetGameEvent(STC_LOBBY_GAME_CREATED, lobby_game->get_id()));
	}
}

void ServerLobbyEvents::on_event_change_game_settings(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	ServerLobbyGame *game = player->get_current_game();
	if(game)
	{
		std::vector<clan::NetGameEventValue> extra_arguments;
		for(size_t i = 0; i < e.get_argument_count(); ++i)
			extra_arguments.push_back(e.get_argument(i));

		try
		{
			game->modify(extra_arguments);
		}
		catch (clan::Exception &e)
		{
			player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Can't change game settings: " + e.message));
		}
	}
	else
	{
		player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "You do not own that game"));
	}
}

void ServerLobbyEvents::on_event_get_available_games(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	games->send_available_lobby_games(player);
}

void ServerLobbyEvents::on_event_join_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	int lobby_game_id = e.get_argument(0);
	ServerLobbyGame *lobby_game = games->get_lobby_game(lobby_game_id);
	if(lobby_game)
	{
		lobby_game->add_player(player);
	}
	else
	{
		player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Game not found"));
	}
}

void ServerLobbyEvents::on_event_leave_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	if(player->get_current_game())
	{
		player->get_current_game()->remove_player(player);
		games->remove_empty_games();
	}
	else
	{
		player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_start_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player)
{
	if(player->get_current_game())
	{
		if(player->get_current_game()->get_owner() == player)
		{
			ServerLobbyGame *lobby_game = player->get_current_game();
			lobby_game->set_state(ServerLobbyGame::playing);

			// TODO: Send a signal or a callback
		}
		else
		{
			player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Can not start a game without owning it"));
		}
	}
	else
	{
		player->send_event(clan::NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Not joined any game"));
	}
}

void ServerLobbyEvents::on_event_add_lobby_message(const clan::NetGameEvent &e, ServerLobbyPlayer *lobby_player)
{
	std::string message = e.get_argument(0);
	if(message.length() > 0)
	{
		server->send_event(clan::NetGameEvent(STC_LOBBY_PLAYER_MESSAGE, lobby_player->get_id(), message));
	}
}
