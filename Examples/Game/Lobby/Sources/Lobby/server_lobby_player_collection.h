
#pragma once

class ServerLobbyPlayer;
class ServerLobbyPlayerInformation;

class ServerLobbyPlayerCollection
{
public:
	ServerLobbyPlayerCollection(CL_NetGameServer *server);
	~ServerLobbyPlayerCollection() {};

	ServerLobbyPlayer *get_player(int player_id);

	ServerLobbyPlayer *create_player(int player_id, CL_NetGameConnection *connection, CL_Callback_1<ServerLobbyPlayerInformation *, CL_NetGameConnection *> &func_create_lobby_player);
	void remove_player(ServerLobbyPlayer *lobby_player);

	int get_count() { return players.size(); }

	void send_available_players(ServerLobbyPlayer *lobby_player);

	void send_event(const CL_NetGameEvent &game_event);

private:
	CL_NetGameServer *server;
	std::vector<ServerLobbyPlayer *> players;
};
