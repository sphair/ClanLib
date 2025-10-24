#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_login_events.h"
#include "server_lobby_events.h"
#include "server_lobby_model.h"
#include "server_lobby_player.h"
#include "server_lobby_game_player_collection.h"
#include "server_game_events.h"
#include "server_game_player.h"

#include "server_lobby_game.h"

class ServerTest
{
public:
	ServerTest(Server *server) : server(server) { TestUsersLeavingAGame(); TestUsersDisconnecting(); }

	void TestSetup()
	{
		model = new ServerLobbyModel(server);

		lobby_player1 = model->players.create_player(new ServerPlayer(0), "Player 1");
		game1 = model->games.create_lobby_game(lobby_player1);

		lobby_player2 = model->players.create_player(new ServerPlayer(0), "Player 2");
		game2 = model->games.create_lobby_game(lobby_player2);

		lobby_player3 = model->players.create_player(new ServerPlayer(0), "Player 3");
		game2->get_players()->add_player(lobby_player3);
	}

	// Similate users leaving a lobby game
	void TestUsersLeavingAGame()
	{
		TestSetup();

		_ASSERT(model->games.get_lobby_games_count() == 2);

		// First player leaves game2
		game2->get_players()->remove_player(lobby_player3);
		model->games.remove_empty_games();

		_ASSERT(model->games.get_lobby_games_count() == 2);

		// Second player leaves game2
		game2->get_players()->remove_player(lobby_player2);
		model->games.remove_empty_games();

		_ASSERT(model->games.get_lobby_games_count() == 1);

		// Player leaves game1
		game1->get_players()->remove_player(lobby_player1);
		model->games.remove_empty_games();

		_ASSERT(model->games.get_lobby_games_count() == 0);
	}

	// Test users disconnecting
	void TestUsersDisconnecting()
	{
		TestSetup();
	
		_ASSERT(model->games.get_lobby_games_count() == 2);
		_ASSERT(model->players.get_count() == 3);

		model->players.remove_player(lobby_player3->player);
		model->games.remove_empty_games();

		_ASSERT(model->games.get_lobby_games_count() == 2);
		_ASSERT(model->players.get_count() == 2);

		model->players.remove_player(lobby_player2->player);
		model->games.remove_empty_games();
	
		_ASSERT(model->games.get_lobby_games_count() == 1);
		_ASSERT(model->players.get_count() == 1);

		model->players.remove_player(lobby_player1->player);
		model->games.remove_empty_games();

		_ASSERT(model->games.get_lobby_games_count() == 0);
		_ASSERT(model->players.get_count() == 0);
	}

private:
	Server *server;

	ServerLobbyModel *model;

	ServerLobbyPlayer *lobby_player1;
	ServerLobbyPlayer *lobby_player2;
	ServerLobbyPlayer *lobby_player3;

	ServerLobbyGame *game1;
	ServerLobbyGame *game2;
};