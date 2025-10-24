
#pragma once

class Server;
class ServerGame;
class ServerLobbyPlayer;
class ServerLobbyGame;
class ServerGame;

class ServerLobbyGameCollection
{
public:
	ServerLobbyGameCollection(Server *server);
	~ServerLobbyGameCollection();

	ServerLobbyGame *create_lobby_game(ServerLobbyPlayer *owner);
	ServerGame *create_game(ServerLobbyGame *lobby_game);

	ServerLobbyGame *get_lobby_game(int lobby_game_id);
	ServerGame *get_running_game(int game_id);

	void remove_empty_games();

	void send_available_lobby_games(ServerLobbyPlayer *player);
	
	int get_lobby_games_count() { return lobby_games.size(); }
	int get_running_games_count() { return running_games.size(); }

private:
	Server *server;

	std::vector<ServerLobbyGame *> lobby_games;
	std::vector<ServerGame *> running_games;

	int next_lobby_game_id;
	int next_game_id;
};
