
#pragma once

class Server;
class ServerLobbyGame;
class ServerLobbyPlayer;
class NetGameEvent;

class ServerLobbyGamePlayerCollection
{
public:
	ServerLobbyGamePlayerCollection(Server *server, ServerLobbyGame *game, ServerLobbyPlayer *owner);
	~ServerLobbyGamePlayerCollection();

	void add_player(ServerLobbyPlayer *player);
	void remove_player(ServerLobbyPlayer *player);
	bool contains_player(ServerLobbyPlayer *player);

	ServerLobbyPlayer *get_owner() const { return owner; }

	int get_count() const { return players.size(); }

	std::vector<ServerLobbyPlayer *> &get_players() { return players; }

	void send_available_players(ServerLobbyPlayer *destination_player);

	void send_event(const NetGameEvent &game_event);

private:
	Server *server;
	ServerLobbyGame *game;
	ServerLobbyPlayer *owner;
	std::vector<ServerLobbyPlayer *> players;
};
