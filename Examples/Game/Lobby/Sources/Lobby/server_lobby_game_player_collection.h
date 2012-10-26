
#pragma once

class ServerLobbyGame;
class ServerLobbyPlayer;

class ServerLobbyGamePlayerCollection
{
public:
	ServerLobbyGamePlayerCollection(CL_NetGameServer *server, ServerLobbyGame *game, ServerLobbyPlayer *owner);
	~ServerLobbyGamePlayerCollection();

	void add_player(ServerLobbyPlayer *player);
	void remove_player(ServerLobbyPlayer *player);
	bool contains_player(ServerLobbyPlayer *player);

	ServerLobbyPlayer *get_owner() const { return owner; }

	int get_count() const { return players.size(); }

	std::vector<ServerLobbyPlayer *> &get_players() { return players; }

	void send_available_players(ServerLobbyPlayer *destination_player);

	void send_event(const CL_NetGameEvent &game_event);

private:
	CL_NetGameServer *server;
	ServerLobbyGame *game;
	ServerLobbyPlayer *owner;
	std::vector<ServerLobbyPlayer *> players;
};
