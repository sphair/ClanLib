
#pragma once

class Server;
class ServerPlayer;
class ServerLobbyPlayer;

class ServerLobbyPlayerCollection
{
public:
	ServerLobbyPlayerCollection(Server *server);
	~ServerLobbyPlayerCollection();

	ServerLobbyPlayer *create_player(ServerPlayer *player, const std::string &player_name);

	void remove_player(ServerPlayer *player);
	void remove_player(ServerLobbyPlayer *lobby_player);

	int get_count() { return players.size(); }

	void send_available_players(ServerLobbyPlayer *lobby_player);

	void send_event(const NetGameEvent &game_event);

private:
	Server *server;
	std::vector<ServerLobbyPlayer *> players;
};
