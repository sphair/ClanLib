
#pragma once

class ServerLobbyGame;
class ServerLobbyPlayerInformation;

class ServerLobbyPlayer
{
public:
	ServerLobbyPlayer(CL_NetGameServer *server, CL_NetGameConnection *connection, int player_id, ServerLobbyPlayerInformation *extra_information);

	CL_NetGameConnection *get_connection() const { return connection; }

	void send_event(const CL_NetGameEvent &game_event);

	int get_id() const { return id; }

	ServerLobbyGame *get_current_game() const { return current_game; }
	void set_current_game(ServerLobbyGame *game);
	void leave_current_game();

	void send_player_info_to_all();
	void send_player_info(ServerLobbyPlayer *destination_player);

private:
	int id;

	ServerLobbyGame *current_game;
	ServerLobbyPlayerInformation *extra_information;

	CL_NetGameConnection *connection;
	CL_NetGameServer *server;
};
