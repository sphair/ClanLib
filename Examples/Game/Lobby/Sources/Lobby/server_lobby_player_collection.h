
#pragma once

class ServerLobbyPlayer;
class ServerLobbyPlayerInformation;

class ServerLobbyPlayerCollection
{
public:
	ServerLobbyPlayerCollection(clan::NetGameServer *server);
	~ServerLobbyPlayerCollection() {};

	ServerLobbyPlayer *get_player(int player_id);

	ServerLobbyPlayer *create_player(int player_id, clan::NetGameConnection *connection, clan::Callback_1<ServerLobbyPlayerInformation *, clan::NetGameConnection *> &func_create_lobby_player);
	void remove_player(ServerLobbyPlayer *lobby_player);

	int get_count() { return players.size(); }

	void send_available_players(ServerLobbyPlayer *lobby_player);

	void send_event(const clan::NetGameEvent &game_event);

private:
	clan::NetGameServer *server;
	std::vector<ServerLobbyPlayer *> players;
};
