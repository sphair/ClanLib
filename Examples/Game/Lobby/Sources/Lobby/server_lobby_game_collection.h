
#pragma once

class ServerLobbyPlayer;
class ServerLobbyGame;
class ServerLobbyGameInformation;

class ServerLobbyGameCollection
{
public:
	ServerLobbyGameCollection(clan::NetGameServer *server);
	~ServerLobbyGameCollection() {};

	ServerLobbyGame *create_lobby_game(ServerLobbyPlayer *owner, clan::Callback_1<ServerLobbyGameInformation *, clan::NetGameConnection *> &func_create_lobby_game);

	ServerLobbyGame *get_lobby_game(int lobby_game_id);

	void remove_empty_games();

	void send_available_lobby_games(ServerLobbyPlayer *player);
	
	int get_lobby_games_count() { return lobby_games.size(); }

private:
	clan::NetGameServer *server;

	std::vector<ServerLobbyGame *> lobby_games;

	int next_lobby_game_id;
};
