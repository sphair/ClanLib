
#include "precomp.h"
#include "lobby_events.h"
#include "lobby_player.h"
#include "lobby_game.h"
#include "client.h"
#include "lobby_view.h"
#include "lobby_player_collection.h"
#include "lobby_game_collection.h"
#include "../Lib/net_events_lobby.h"

LobbyEvents::LobbyEvents(Client *client)
: client(client)
{
	lobby_events.func_event(STC_LOBBY_PLAYER_LOGGED_IN).set(this, &LobbyEvents::on_event_player_logged_in);
	lobby_events.func_event(STC_LOBBY_PLAYER_LOGGED_OUT).set(this, &LobbyEvents::on_event_player_logged_out);
	lobby_events.func_event(STC_LOBBY_GAME_CREATED).set(this, &LobbyEvents::on_event_game_created);
	lobby_events.func_event(STC_LOBBY_GAME_REMOVED).set(this, &LobbyEvents::on_event_game_removed);
	lobby_events.func_event(STC_LOBBY_GAME_INFO).set(this, &LobbyEvents::on_event_game_info);
	lobby_events.func_event(STC_LOBBY_NO_GAMES_AVAILABLE).set(this, &LobbyEvents::on_event_no_games_available);
	lobby_events.func_event(STC_LOBBY_PLAYER_JOINED_GAME).set(this, &LobbyEvents::on_event_player_joined_game);
	lobby_events.func_event(STC_LOBBY_PLAYER_LEFT_GAME).set(this, &LobbyEvents::on_event_player_left_game);
	lobby_events.func_event(STC_LOBBY_GAME_STARTED).set(this, &LobbyEvents::on_event_game_started);
	lobby_events.func_event(STC_LOBBY_PLAYER_MESSAGE).set(this, &LobbyEvents::on_event_player_message);
	lobby_events.func_event(STC_LOBBY_ERROR_MESSAGE).set(this, &LobbyEvents::on_event_error_message);

	player_collection.reset(new LobbyPlayerCollection());
	game_collection.reset(new LobbyGameCollection());
}

LobbyEvents::~LobbyEvents()
{
}

void LobbyEvents::on_event_game_created(const NetGameEvent &e)
{
	int game_id = e.get_argument(0);
	std::string game_name = e.get_argument(1);
	std::string map_name = e.get_argument(2);
	int max_players = e.get_argument(3);

	LobbyGame *game = game_collection->create_game(game_id, game_name, map_name, max_players, LobbyGame::lobby);
	if(game)
	{
		if (client->get_lobby_view())
		{
			client->get_lobby_view()->add_game(game);
			client->get_lobby_view()->create_game(game);
		}
	}
}

void LobbyEvents::on_event_game_info(const NetGameEvent &e)
{
	int game_id = e.get_argument(0);
	std::string game_name = e.get_argument(1);
	std::string map_name = e.get_argument(2);
	int max_players = e.get_argument(3);
	int int_state = e.get_argument(4);
	LobbyGame::State state = (LobbyGame::State)int_state;
	LobbyGame *game = game_collection->create_game(game_id, game_name, map_name, max_players, state);

	if(game)
	{
		if (client->get_lobby_view())
		{
			client->get_lobby_view()->add_game(game);
		}
	}
}

void LobbyEvents::on_event_game_removed(const NetGameEvent &e)
{
	int game_id = e.get_argument(0);
	LobbyGame *game = game_collection->get_game(game_id);

	if(game)
	{
		if (client->get_lobby_view())
			client->get_lobby_view()->remove_game(game);

		game_collection->remove_game(game_id);
	}
}

void LobbyEvents::on_event_no_games_available(const NetGameEvent &e)
{
//	if (client->get_lobby_view())
//		client->get_lobby_view()->add_info_message("No games available on server");
}

void LobbyEvents::on_event_game_started(const NetGameEvent &e)
{
	int game_id = e.get_argument(0);
	LobbyGame *game = game_collection->get_game(game_id);
	if(game)
	{
		game->set_state(LobbyGame::playing);
		client->get_lobby_view()->update_game(game);
	}
}

LobbyPlayer *LobbyEvents::get_player(int player_id) const
{
	return player_collection->get_player(player_id);
}

void LobbyEvents::on_event_player_logged_in(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	std::string player_name = e.get_argument(1);

	LobbyPlayer *player = player_collection->create_player(player_id, player_name);

	if (client->get_lobby_view())
		client->get_lobby_view()->player_logged_in(player);
}

void LobbyEvents::on_event_player_logged_out(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);

	LobbyPlayer *player = player_collection->get_player(player_id);

	if(player)
	{
		if (client->get_lobby_view())
			client->get_lobby_view()->player_logged_out(player);

		game_collection->remove_player(player);
		player_collection->remove_player(player_id);
	}
}

void LobbyEvents::on_event_player_joined_game(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	int game_id = e.get_argument(1);

	LobbyPlayer *player = player_collection->get_player(player_id);
	LobbyGame *game = game_collection->get_game(game_id);

	if(game && player)
	{
		if(game->contains_player(player) == false)
		{
			game->add_player(player);

			if (client->get_lobby_view())
				client->get_lobby_view()->player_joined_game(game, player);
		}
	}
}

void LobbyEvents::on_event_player_left_game(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	int game_id = e.get_argument(1);

	LobbyPlayer *player = player_collection->get_player(player_id);
	LobbyGame *game = game_collection->get_game(game_id);

	if(game && player)
	{
		game->remove_player(player);

		if (client->get_lobby_view())
			client->get_lobby_view()->player_left_game(game, player);
	}
}

void LobbyEvents::on_event_player_message(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	std::string message = e.get_argument(1);

	LobbyPlayer *player = player_collection->get_player(player_id);

	if(client->get_lobby_view() && player)
		client->get_lobby_view()->add_chat_message(player, message);
}

void LobbyEvents::on_event_error_message(const NetGameEvent &e)
{
	std::string message = e.get_argument(0);

	if(client->get_lobby_view())
		client->get_lobby_view()->add_info_message(message);
}
